//
//  GLSLProgram.hpp
//  OpenGLWithGlfw
//
//  Created by qyt on 16/3/8.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef GLSLProgram_hpp
#define GLSLProgram_hpp

#include <stdio.h>
#include <unordered_map>
#include <OpenGL/gl.h>
#include <string>

namespace qyt
{
    struct GLVertexAttribute
    {
        GLuint      _index;
        GLint       _size;
        GLenum      _type;
        std::string _name;
    };
    
    struct __uniform
    {
        GLuint      _location;
        GLint       _size;
        GLenum      _type;
        std::string _name;
    };
    
    class GLSLProgram
    {
    public:
        enum
        {
            _vertex_att_pos,
            _vertex_att_color,
            _vertex_att_tex_coord0,
            _vertex_att_tex_coord1,
            _vertex_att_tex_coord2,
            _vertex_att_tex_coord3,
            _vertex_att_normal,
            
            _vertex_att_max,
        };
        
        enum
        {
            _uniform_ambient_color,
            _uniform_p_matrix,
            _uniform_mv_matrix,
            _uniform_mvp_matrix,
            _uniform_normal_matrix,
            _uniform_sampler0,
            _uniform_sampler1,
            _uniform_sampler2,
            _uniform_sampler3,
            
            _uniform_max,
        };
        
        static const char* __uniform_p_matrix_name;
        static const char* __uniform_mv_matrix_name;
        static const char* __uniform_mvp_matrix_name;
        static const char* __uniform_normal_matrix_name;
        static const char* __uniform_sampler0_name;
        static const char* __uniform_sampler1_name;
        static const char* __uniform_sampler2_name;
        static const char* __uniform_sampler3_name;
        
        static const char* __vertex_att_pos_name;
        static const char* __vertex_att_color_name;
        static const char* __vertex_att_tex_coord0_name;
        static const char* __vertex_att_tex_coord1_name;
        static const char* __vertex_att_tex_coord2_name;
        static const char* __vertex_att_tex_coord3_name;
        static const char* __vertex_att_normal_name;
        
        GLSLProgram();
        virtual ~GLSLProgram();
        static GLSLProgram* createWithSource(const char* __vShaderSrc, const char* __fShaderSrc);
        static GLSLProgram* createWithFile(const std::string& vertex_file, const std::string& fragment_file);
        
        bool initWithSource(const char* __vShaderSrc, const char* __fShaderSrc);
        
        __uniform* getUniform(const std::string& __name);
        GLVertexAttribute * getVertexAttrib(const std::string& __name);
        
        GLint getAttribLocation(const std::string &attributeName) const;
        GLint getUniformLocation(const std::string &attributeName) const;
        
        void use();
        
        void uniformMatrix4fv(GLint __location,
                              const GLfloat* __matrixArray,
                              unsigned int __numberOfMatrices);
        
        void uniform2fv(GLint __location,
                        const GLfloat* __floats,
                        unsigned int __numberOfArrays);
        
        void uniform3fv(GLint __location,
                        const GLfloat* __floats,
                        unsigned int __numberOfArrays);
        
        void uniform4fv(GLint __location,
                        const GLfloat* __floats,
                        unsigned int __numberOfArrays);
        
    protected:
        GLuint      _program;
        GLuint      _vertShader;
        GLuint      _fragShader;
        
        GLint       _builtInUniforms[_uniform_max];
        
        std::unordered_map<std::string, __uniform> _userUniforms;
        std::unordered_map<std::string, GLVertexAttribute> _vertexAttribs;
        
        void updateUniforms();
        bool link();
        bool compileShader(GLuint* __shader, GLenum __type, const GLchar* __source);
        void bindPredefinedVertexAttribs();
        void parseUniforms();
        void parseVertexAttribs();

    };
}

#endif /* GLSLProgram_hpp */
