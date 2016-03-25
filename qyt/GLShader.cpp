//
// Created by qyt on 16/3/25.
//

#include "GLShader.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>
#include "FileUtils.hpp"

#define MAX_LOG_SIZE 2048

static bool  check_gl_error()
{
    GLenum  error = glGetError();
    return error == GL_NO_ERROR;
}

namespace qyt
{
    const char* GLShaderProgram::uniform_p_matrix_name            = "qyt_pMatrix";
    const char* GLShaderProgram::uniform_mv_matrix_name           = "qyt_mvMatrix";
    const char* GLShaderProgram::uniform_mvp_matrix_name          = "qyt_mvpMatrix";
    const char* GLShaderProgram::uniform_normal_matrix_name       = "qyt_normalMatrix";
    const char* GLShaderProgram::uniform_sampler0_name            = "qyt_sampler0";
    const char* GLShaderProgram::uniform_sampler1_name            = "qyt_sampler1";
    const char* GLShaderProgram::uniform_sampler2_name            = "qyt_sampler2";
    const char* GLShaderProgram::uniform_sampler3_name            = "qyt_sampler3";

    const char* GLShaderProgram::vertex_att_pos_name              = "qyt_position";
    const char* GLShaderProgram::vertex_att_color_name            = "qyt_color";
    const char* GLShaderProgram::vertex_att_tex_coord0_name       = "qyt_text_coord0";
    const char* GLShaderProgram::vertex_att_tex_coord1_name       = "qyt_text_coord1";
    const char* GLShaderProgram::vertex_att_tex_coord2_name       = "qyt_text_coord2";
    const char* GLShaderProgram::vertex_att_tex_coord3_name       = "qyt_text_coord3";
    const char* GLShaderProgram::vertex_att_normal_name           = "qyt_normal";

    GLShaderProgram* GLShaderProgram::createWithSource(const std::string &v_shader_src,
                                                       const std::string &f_shader_src)
    {
        GLShaderProgram* ret = new (std::nothrow) GLShaderProgram();
        if(ret && ret->initWithSource(v_shader_src.c_str(), f_shader_src.c_str()))
        {
            ret->link();
            ret->updateUniforms();
            return ret;
        }
        delete ret;
        ret = nullptr;
        return nullptr;
    }

    GLShaderProgram* GLShaderProgram::createWithFile(const std::string &v_shader_file,
                                                     const std::string &f_shader_file)
    {
        auto file_util = FileUtils::getInstance();
        std::string v_full_path = file_util->fullPathForFilename(v_shader_file);
        std::string f_full_path = file_util->fullPathForFilename(f_shader_file);
        std::string vertex_shader_code = file_util->getStringFromFile(v_full_path);
        std::string fragment_shader_code = file_util->getStringFromFile(f_full_path);

        return GLShaderProgram::createWithSource(vertex_shader_code, fragment_shader_code);
    }


    GLShaderProgram::GLShaderProgram():
    program_(0),
    vertex_shader_(0),
    fragment_shader_(0)
    {
        memset(built_in_uniforms_, 0, sizeof(built_in_uniforms_));
    }

    GLShaderProgram::~GLShaderProgram()
    {
        if(vertex_shader_ != 0)
            glDeleteShader(vertex_shader_);
        if(fragment_shader_ != 0)
            glDeleteShader(fragment_shader_);
        if(program_)
            glDeleteProgram(program_);
    }

    bool GLShaderProgram::initWithSource(const char *v_shader_src, const char *f_shader_src)
    {
        program_ = glCreateProgram();
        vertex_shader_ = fragment_shader_ = 0;

        if(!v_shader_src || !this->complile(&vertex_shader_, GL_VERTEX_SHADER, v_shader_src))
            return false;
        if(!f_shader_src || !this->complile(&fragment_shader_, GL_FRAGMENT_SHADER, f_shader_src))
            return false;
        glAttachShader(program_, vertex_shader_);
        glAttachShader(program_, fragment_shader_);

        return check_gl_error();
    }

    void GLShaderProgram::bindPredefinedVertexAttribs()
    {
        static const struct {
            const char *attributeName;
            int location;
        } AttributeLocations[] =
                {
                    {GLShaderProgram::vertex_att_pos_name,        GLShaderProgram::VERTEX_ATTRIBUTE_POSITION},
                    {GLShaderProgram::vertex_att_color_name,      GLShaderProgram::VERTEX_ATTRIBUTE_COLOR},
                    {GLShaderProgram::vertex_att_tex_coord0_name, GLShaderProgram::VERTEX_ATTRIBUTE_TEXTURE_COORD0},
                    {GLShaderProgram::vertex_att_tex_coord1_name, GLShaderProgram::VERTEX_ATTRIBUTE_TEXTURE_COORD1},
                    {GLShaderProgram::vertex_att_tex_coord2_name, GLShaderProgram::VERTEX_ATTRIBUTE_TEXTURE_COORD2},
                    {GLShaderProgram::vertex_att_tex_coord3_name, GLShaderProgram::VERTEX_ATTRIBUTE_TEXTURE_COORD3},
                    {GLShaderProgram::vertex_att_normal_name,     GLShaderProgram::VERTEX_ATTRIBUTE_NORMAL}
                };
        const int size = sizeof(AttributeLocations) / sizeof(AttributeLocations[0]);
        for(int i=0; i<size;i++)
            glBindAttribLocation(program_, AttributeLocations[i].location, AttributeLocations[i].attributeName);
    }

    bool GLShaderProgram::complile(GLuint *shader, GLenum type, const char *source)
    {
        if(!source) return false;
        GLint status;
        *shader = glCreateShader(type);
        const GLchar *sources[] = {
                "#version 330 core\n"
                "uniform mat4 qyt_pMatrix;\n"
                "uniform mat4 qyt_mvMatrix;\n"
                "uniform mat4 qyt_mvpMatrix;\n"
                "uniform mat3 qyt_normalMatrix;\n"
                "uniform sampler2D qyt_sampler0;\n"
                "uniform sampler2D qyt_sampler1;\n"
                "uniform sampler2D qyt_sampler2;\n"
                "uniform sampler2D qyt_sampler3;\n",
                source
        };
        glShaderSource(*shader, sizeof(sources)/ sizeof(*sources), sources, nullptr);
        glCompileShader(*shader);

        glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            GLchar infoLog[MAX_LOG_SIZE];
            glGetShaderInfoLog(*shader, MAX_LOG_SIZE, nullptr, infoLog);
            std::cerr << infoLog;
            glDeleteShader(*shader);
            return false;
        }
        return true;
    }

    bool GLShaderProgram::link()
    {
        assert(program_ != 0);
        GLint status = GL_TRUE;
        bindPredefinedVertexAttribs();
        glLinkProgram(program_);

        if(!check_gl_error())
            return false;

        parseVertexAttribs();
        parseUniforms();

        if (vertex_shader_) glDeleteShader(vertex_shader_);
        if (fragment_shader_) glDeleteShader(fragment_shader_);

        vertex_shader_= fragment_shader_= 0;
        return true;
    }

    void GLShaderProgram::parseUniforms()
    {
        user_uniforms_.clear();
        GLint active_uniforms;
        glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &active_uniforms);
        if(active_uniforms > 0)
        {
            GLint length;
            glGetProgramiv(program_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
            if(length > 0)
            {
                GLUniform uniform;

                GLchar* uniform_name = (GLchar*)alloca(length + 1);

                for(int i = 0; i < active_uniforms; ++i)
                {
                    glGetActiveUniform(program_, i, length, nullptr, &uniform.size, &uniform.type, uniform_name);
                    uniform_name[length] = '\0';
                    if(strncmp("qyt", uniform_name, 3) != 0) {
                        if(length > 3)
                        {
                            char* c = strrchr(uniform_name, '[');
                            if(c)
                            {
                                *c = '\0';
                            }
                        }
                        uniform.name = std::string(uniform_name);
                        uniform.location = glGetUniformLocation(program_, uniform_name);
                        GLenum __gl_error_code = glGetError();

                        assert(__gl_error_code == GL_NO_ERROR);

                        user_uniforms_[uniform.name] = uniform;
                    }
                }
            }
        }
        else
        {
            GLchar error_log[1024];
            glGetProgramInfoLog(program_, sizeof(error_log), NULL, error_log);
            std::cerr <<"Error linking shader program: " << error_log << std::endl;
        }
    }

    void GLShaderProgram::parseVertexAttribs()
    {
        user_attributes_.clear();

        GLint activeAttributes;
        GLint length;
        glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
        if(activeAttributes > 0)
        {
            GLVertexAttribute attribute;

            glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);
            if(length > 0)
            {
                GLchar* attribName = (GLchar*) alloca(length + 1);

                for(GLuint i = 0; i < activeAttributes; ++i)
                {
                    glGetActiveAttrib(program_, i, length, nullptr, &attribute.size, &attribute.type, attribName);
                    attribName[length] = '\0';
                    attribute.name = std::string(attribName);

                    attribute.location = glGetAttribLocation(program_, attribName);
                    user_attributes_[attribute.name] = attribute;
                }
            }
        }
        else
        {
            GLchar ErrorLog[1024];
            glGetProgramInfoLog(program_, sizeof(ErrorLog), NULL, ErrorLog);
            std::cerr <<"Error linking shader program: " << ErrorLog << std::endl;
        }
    }
}
