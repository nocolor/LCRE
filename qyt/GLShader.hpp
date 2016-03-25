//
// Created by qyt on 16/3/25.
//

#ifndef OPENGL_GLSHADER_HPP
#define OPENGL_GLSHADER_HPP

#include <stdio.h>
#include <unordered_map>
#include <OpenGL/gl.h>
#include <string>

namespace qyt
{
    /**
     * 表示一个顶点属性(GLSL)中的attribute
     */
    struct GLSLDataProperty
    {
        GLuint      location;       //location
        GLint       size;           //0, 1, 2, 3, 4
        GLenum      type;           //GL_FLOAT, GL_UNSIGNED_BYTE etc.
        std::string name;
    };

    typedef GLSLDataProperty GLVertexAttribute;
    typedef GLSLDataProperty GLUniform;

    /**
     * 封装OpenGL着色器程序
     * 主要用于快速创建OpenGL着色器,绑定内置着色器变量.
     * 使用GLShaderProgram对象创建的着色器程序,必须符合预置的约定
     *
     */
    class GLShaderProgram
    {
    public:
        //预置的attribute位置
        enum
        {
            VERTEX_ATTRIBUTE_POSITION = 0,
            VERTEX_ATTRIBUTE_COLOR,
            VERTEX_ATTRIBUTE_TEXTURE_COORD0,
            VERTEX_ATTRIBUTE_TEXTURE_COORD1,
            VERTEX_ATTRIBUTE_TEXTURE_COORD2,
            VERTEX_ATTRIBUTE_TEXTURE_COORD3,
            VERTEX_ATTRIBUTE_NORMAL,
            VERTEX_ATTRIBUTE_MAX,
        };
        //预置的uniform位置
        enum
        {
            UNIFORM_AMBIENT_COLOR,
            UNIFORM_P_MATRIX,
            UNIFORM_MV_MATRIX,
            UNIFORM_MVP_MATRIX,
            UNIFORM_NORMAL_MATRIX,
            UNIFORM_SAMPLER_0,
            UNIFORM_SAMPLER_1,
            UNIFORM_SAMPLER_2,
            UNIFORM_SAMPLER_3,
            UNIFORM_MAX,
        };

        //定义内部预置的变量名称,这些名称会被使用在经由GLShaderProgram对象
        //创建的着色器程序中
        static const char* uniform_p_matrix_name;
        static const char* uniform_mv_matrix_name;
        static const char* uniform_mvp_matrix_name;
        static const char* uniform_normal_matrix_name;
        static const char* uniform_sampler0_name;
        static const char* uniform_sampler1_name;
        static const char* uniform_sampler2_name;
        static const char* uniform_sampler3_name;

        static const char* vertex_att_pos_name;
        static const char* vertex_att_color_name;
        static const char* vertex_att_tex_coord0_name;
        static const char* vertex_att_tex_coord1_name;
        static const char* vertex_att_tex_coord2_name;
        static const char* vertex_att_tex_coord3_name;
        static const char* vertex_att_normal_name;

        GLShaderProgram();
        virtual ~GLShaderProgram();

        static GLShaderProgram* createWithSource(const std::string& v_shader_src,
                                                 const std::string& f_shader_src);

        static GLShaderProgram* createWithFile(const std::string& v_shader_file,
                                               const std::string& f_shader_file);

        GLVertexAttribute* getVertexAttribute(const std::string& attribute_name) const;
        GLUniform* getUniformLcation(const std::string& uniform_name) const;


        template <typename type>
        void setUniformMatrix4(GLint location, const type* matrix, unsigned int size);

        template <typename type, int size>
        void setUniform(GLint location, const type* vec);

    protected:
        GLuint program_;
        GLuint vertex_shader_;
        GLuint fragment_shader_;

        GLint  built_in_uniforms_[UNIFORM_MAX];

        std::unordered_map<std::string, GLUniform> user_uniforms_;
        std::unordered_map<std::string, GLVertexAttribute> user_attributes_;

        bool initWithSource(const char* v_shader_src, const char* f_shader_src);
        void updateUniforms();
        bool complile(GLuint* shader, GLenum type, const char* source);
        bool link();

        void bindPredefinedVertexAttribs();
        void parseUniforms();
        void parseVertexAttribs();
    };

}

#endif //OPENGL_GLSHADER_HPP
