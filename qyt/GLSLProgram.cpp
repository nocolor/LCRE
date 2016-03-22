//
//  GLSLProgram.cpp
//  OpenGLWithGlfw
//
//  Created by qyt on 16/3/8.
//  Copyright © 2016年 qyt. All rights reserved.
//

#include "GLSLProgram.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>
#include "FileUtils.hpp"
#define MAX_LOG_LENTH 2048

namespace qyt
{
    const char* GLSLProgram::__uniform_p_matrix_name            = "__qyt_pMatrix";
    const char* GLSLProgram::__uniform_mv_matrix_name           = "__qyt_mvMatrix";
    const char* GLSLProgram::__uniform_mvp_matrix_name          = "__qyt_mvpMatrix";
    const char* GLSLProgram::__uniform_normal_matrix_name       = "__qyt_normalMatrix";
    const char* GLSLProgram::__uniform_sampler0_name            = "__qyt_sampler0";
    const char* GLSLProgram::__uniform_sampler1_name            = "__qyt_sampler1";
    const char* GLSLProgram::__uniform_sampler2_name            = "__qyt_sampler2";
    const char* GLSLProgram::__uniform_sampler3_name            = "__qyt_sampler3";
    
    const char* GLSLProgram::__vertex_att_pos_name              = "__qyt_position";
    const char* GLSLProgram::__vertex_att_color_name            = "__qyt_color";
    const char* GLSLProgram::__vertex_att_tex_coord0_name       = "__qyt_text_coord0";
    const char* GLSLProgram::__vertex_att_tex_coord1_name       = "__qyt_text_coord1";
    const char* GLSLProgram::__vertex_att_tex_coord2_name       = "__qyt_text_coord2";
    const char* GLSLProgram::__vertex_att_tex_coord3_name       = "__qyt_text_coord3";
    const char* GLSLProgram::__vertex_att_normal_name           = "__qyt_normal";
    
    
    GLSLProgram* GLSLProgram::createWithSource(const char *__vShaderSrc, const char *__fShaderSrc)
    {
        GLSLProgram* ret = new(std::nothrow) GLSLProgram();
        if(ret && ret->initWithSource(__vShaderSrc, __fShaderSrc))
        {
            ret->link();
            ret->updateUniforms();
            return ret;
        }
        delete ret;
        ret = nullptr;
        return nullptr;
    }
    
    GLSLProgram* GLSLProgram::createWithFile(const std::string &vertex_file, const std::string &fragment_file)
    {
        auto fileUtil = FileUtils::getInstance();
        std::string VertexShaderCode = fileUtil->getStringFromFile(fileUtil->fullPathForFilename(vertex_file));
        std::string FragmentShaderCode = fileUtil->getStringFromFile(fileUtil->fullPathForFilename(fragment_file));
        
        return createWithSource(VertexShaderCode.c_str(), FragmentShaderCode.c_str());
    }

    GLSLProgram::GLSLProgram():
    _program(0)
    , _vertShader(0)
    , _fragShader(0)
    {
        memset(_builtInUniforms, 0, sizeof(_builtInUniforms));
    }
    
    GLSLProgram::~GLSLProgram()
    {
        if (_vertShader)
        {
            glDeleteShader(_vertShader);
        }
        
        if (_fragShader)
        {
            glDeleteShader(_fragShader);
        }
        
        _vertShader = _fragShader = 0;
        
        if (_program)
        {
            glDeleteProgram(_program);
        }
    }
    
    bool GLSLProgram::initWithSource(const char *__vShaderSrc, const char *__fShaderSrc)
    {
        _program = glCreateProgram();
        _vertShader = _fragShader = 0;
        if(__vShaderSrc)
            if(!compileShader(&_vertShader, GL_VERTEX_SHADER, __vShaderSrc))
                return false;
        if(__fShaderSrc)
            if(!compileShader(&_fragShader, GL_FRAGMENT_SHADER, __fShaderSrc))
                return false;
        if(_vertShader)
            glAttachShader(_program, _vertShader);
        if(_fragShader)
            glAttachShader(_program, _fragShader);
        return true;
    }
    
    void GLSLProgram::bindPredefinedVertexAttribs()
    {
        static const struct {
            const char *attributeName;
            int location;
        } attribute_locations[] =
        {
            {GLSLProgram::__vertex_att_pos_name,        GLSLProgram::_vertex_att_pos},
            {GLSLProgram::__vertex_att_color_name,      GLSLProgram::_vertex_att_color},
            {GLSLProgram::__vertex_att_tex_coord0_name, GLSLProgram::_vertex_att_tex_coord0},
            {GLSLProgram::__vertex_att_tex_coord1_name, GLSLProgram::_vertex_att_tex_coord1},
            {GLSLProgram::__vertex_att_tex_coord2_name, GLSLProgram::_vertex_att_tex_coord2},
            {GLSLProgram::__vertex_att_tex_coord3_name, GLSLProgram::_vertex_att_tex_coord3},
            {GLSLProgram::__vertex_att_normal_name,     GLSLProgram::_vertex_att_normal}
        };
        const int size = sizeof(attribute_locations) / sizeof(attribute_locations[0]);
        for(int i=0; i<size;i++)
            glBindAttribLocation(_program, attribute_locations[i].location, attribute_locations[i].attributeName);
    }
    
    bool GLSLProgram::compileShader(GLuint *__shader, GLenum __type, const GLchar *__source)
    {
        GLint status;
        
        if (!__source)
        {
            return false;
        }
        
        *__shader = glCreateShader(__type);
        
        const GLchar *sources[] = {
            "#version 330 core\n"
            "uniform mat4 __qyt_pMatrix;\n"
            "uniform mat4 __qyt_mvMatrix;\n"
            "uniform mat4 __qyt_mvpMatrix;\n"
            "uniform mat3 __qyt_normalMatrix;\n"
            "uniform sampler2D __qyt_sampler0;\n"
            "uniform sampler2D __qyt_sampler1;\n"
            "uniform sampler2D __qyt_sampler2;\n"
            "uniform sampler2D __qyt_sampler3;\n",
            __source
        };
        
        glShaderSource(*__shader, sizeof(sources)/ sizeof(*sources), sources, nullptr);

        glCompileShader(*__shader);
        
        glGetShaderiv(*__shader, GL_COMPILE_STATUS, &status);
        
        if (!status)
        {
            GLchar infoLog[MAX_LOG_LENTH];
            glGetShaderInfoLog(*__shader, MAX_LOG_LENTH, nullptr, infoLog);
            std::cerr << infoLog;
            glDeleteShader(*__shader);
            return false;
        }
        return (status == GL_TRUE);
    }

    bool GLSLProgram::link()
    {
        assert(_program != 0);
        GLint status = GL_TRUE;
        bindPredefinedVertexAttribs();
        glLinkProgram(_program);
        
        parseVertexAttribs();
        parseUniforms();
        
        if (_vertShader) glDeleteShader(_vertShader);
        if (_fragShader) glDeleteShader(_fragShader);
        
        _vertShader = _fragShader = 0;
        return (status == GL_TRUE);
    }
    
    void GLSLProgram::parseUniforms()
    {
        _userUniforms.clear();
        GLint activeUniforms;
        glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &activeUniforms);
        if(activeUniforms > 0)
        {
            GLint length;
            glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
            if(length > 0)
            {
                __uniform uniform;
                
                GLchar* uniformName = (GLchar*)alloca(length + 1);
                
                for(int i = 0; i < activeUniforms; ++i)
                {
                    glGetActiveUniform(_program, i, length, nullptr, &uniform._size, &uniform._type, uniformName);
                    uniformName[length] = '\0';
                    if(strncmp("__qyt", uniformName, 5) != 0) {
                        if(length > 3)
                        {
                            char* c = strrchr(uniformName, '[');
                            if(c)
                            {
                                *c = '\0';
                            }
                        }
                        uniform._name = std::string(uniformName);
                        uniform._location = glGetUniformLocation(_program, uniformName);
                        GLenum __gl_error_code = glGetError();
                        
                        assert(__gl_error_code == GL_NO_ERROR);
                        
                        _userUniforms[uniform._name] = uniform;
                    }
                }
            }
        }
        else
        {
            GLchar ErrorLog[1024];
            glGetProgramInfoLog(_program, sizeof(ErrorLog), NULL, ErrorLog);
            std::cerr <<"Error linking shader program: " << ErrorLog << std::endl;
        }
    }
    
    void GLSLProgram::parseVertexAttribs()
    {
        _vertexAttribs.clear();
        
        GLint activeAttributes;
        GLint length;
        glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
        if(activeAttributes > 0)
        {
            __vertex_attrib attribute;
            
            glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);
            if(length > 0)
            {
                GLchar* attribName = (GLchar*) alloca(length + 1);
                
                for(GLuint i = 0; i < activeAttributes; ++i)
                {
                    glGetActiveAttrib(_program, i, length, nullptr, &attribute._size, &attribute._type, attribName);
                    attribName[length] = '\0';
                    attribute._name = std::string(attribName);
                    
                    attribute._index = glGetAttribLocation(_program, attribName);
                    _vertexAttribs[attribute._name] = attribute;
                }
            }
        }
        else
        {
            GLchar ErrorLog[1024];
            glGetProgramInfoLog(_program, sizeof(ErrorLog), NULL, ErrorLog);
            std::cerr <<"Error linking shader program: " << ErrorLog << std::endl;
        }
    }

    void GLSLProgram::updateUniforms()
    {
        _builtInUniforms[_uniform_p_matrix]         = glGetUniformLocation(_program, __uniform_p_matrix_name);
        _builtInUniforms[_uniform_mv_matrix]        = glGetUniformLocation(_program, __uniform_mv_matrix_name);
        _builtInUniforms[_uniform_mvp_matrix]       = glGetUniformLocation(_program, __uniform_mvp_matrix_name);
        _builtInUniforms[_uniform_normal_matrix]    = glGetUniformLocation(_program, __uniform_normal_matrix_name);
        _builtInUniforms[_uniform_sampler0]         = glGetUniformLocation(_program, __uniform_sampler0_name);
        _builtInUniforms[_uniform_sampler1]         = glGetUniformLocation(_program, __uniform_sampler1_name);
        _builtInUniforms[_uniform_sampler2]         = glGetUniformLocation(_program, __uniform_sampler2_name);
        _builtInUniforms[_uniform_sampler3]         = glGetUniformLocation(_program, __uniform_sampler3_name);
    }
    
    __uniform* GLSLProgram::getUniform(const std::string &__name)
    {
        const auto itr = _userUniforms.find(__name);
        if( itr != _userUniforms.end())
            return &itr->second;
        return nullptr;
    }
    
    __vertex_attrib* GLSLProgram::getVertexAttrib(const std::string &__name)
    {
        const auto itr = _vertexAttribs.find(__name);
        if( itr != _vertexAttribs.end())
            return &itr->second;
        return nullptr;
    }

    GLint GLSLProgram::getUniformLocation(const std::string &attributeName) const
    {
        return glGetUniformLocation(_program, attributeName.c_str());
    }
    
    GLint GLSLProgram::getAttribLocation(const std::string &attributeName) const
    {
        return glGetAttribLocation(_program, attributeName.c_str());
    }
    
    void GLSLProgram::uniformMatrix4fv(GLint __location,
                                     const GLfloat *__matrixArray,
                                     unsigned int __numberOfMatrices)
    {
        glUniformMatrix4fv(__location, (GLsizei)__numberOfMatrices, GL_FALSE, __matrixArray);
    }
    
    void GLSLProgram::uniform2fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
    {
        glUniform2fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
    
    void GLSLProgram::uniform3fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
    {
        glUniform3fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
    
    void GLSLProgram::uniform4fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
    {
        glUniform4fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
    
    void GLSLProgram::use()
    {
        glUseProgram(_program);
    }

}