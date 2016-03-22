//
//  batch.hpp
//  lcr
//
//  Created by qyt on 16/1/28.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef batch_hpp
#define batch_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <OpenGL/gl.h>

#include "math/math.hpp"

namespace qyt
{
    
    /**
     @class Batch
     @brief “批次”类，一个批次对应一次OpenGL绘图调用的所有数据（及正确的组织形式）。
     @description
            一个批次的初始化类似于早期OpenGL调用，glBegin和glEnd之间的函数。例如
            glVertex2/3/4*和glNormal2/3/4*这类。这些调用会将函数提供的数据以vao
            的形式组织起来，内部使用vbo进行缓存。用户提供一个GL枚举来标识数据组织和
            绘制的方式。
     @attention
            顶点函数vertex3f必须最后调用，因为这个函数真正标识一个顶点数据的结束。
     */
    class Batch
    {
    public:
        Batch();
        virtual ~Batch();
        void begin(GLenum _primitive, GLuint _verts, GLuint _textureUnits = 0);
        void end();
        virtual void draw();
        
        void vertex3f(GLfloat _x, GLfloat _y, GLfloat _z);
        void vertex3fv(const vec3& _v);
        
        void normal3f(GLfloat _x, GLfloat _y, GLfloat _z);
        void normal3fv(const vec3& _v);
        
        void color4f(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a);
        void color4fv(const vec4& _c);
        
        void texCoord2f(GLuint _texture, GLclampf _s, GLclampf _t);
        void texCoord2v(GLuint _texture, const vec2& _tc);
    protected:
        GLenum primitiveType;
        GLuint vbo_vertexArray;
        GLuint vbo_normalArray;
        GLuint vbo_colorArray;
        GLuint *vbo_textureCoordArray;
        GLuint vao;
        
        GLuint needToBuild;
        GLuint nNumTextureUnits;
        GLuint builded;
        
        bool finished;
        
        vec3* pVerts;
        vec3* pNormals;
        vec4* pColors;
        vec2** pTexCoords;
    };
}

#endif /* batch_hpp */
