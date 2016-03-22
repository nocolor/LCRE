//
//  batch.cpp
//  lcr
//
//  Created by qyt on 16/1/28.
//  Copyright © 2016年 qyt. All rights reserved.
//

#include "batch.hpp"
#include "qyt/GLSLProgram.hpp"
#include "math/math.hpp"

namespace qyt
{
    Batch::Batch():nNumTextureUnits(0),needToBuild(0),pVerts(nullptr),pNormals(nullptr),pColors(nullptr),vbo_textureCoordArray(nullptr),builded(0),
    vbo_vertexArray(0),vbo_normalArray(0),vbo_colorArray(0),finished(false),pTexCoords(nullptr)
    {}
    
    Batch::~Batch()
    {
        if(vbo_vertexArray != 0)
            glDeleteBuffers(1, &vbo_vertexArray);
        if(vbo_normalArray != 0)
            glDeleteBuffers(1, &vbo_normalArray);
        if(vbo_colorArray != 0)
            glDeleteBuffers(1, &vbo_colorArray);
        glDeleteVertexArrays(1, &vao);
        
        for(unsigned int i = 0; i < nNumTextureUnits; i++)
            glDeleteBuffers(1, &vbo_textureCoordArray[i]);
        
        if (vbo_textureCoordArray) delete [] vbo_textureCoordArray;
        if (pTexCoords) delete [] pTexCoords;
    }
    
    void Batch::begin(GLenum _primitive, GLuint _verts, GLuint _textureUnits)
    {
        primitiveType = _primitive;
        needToBuild = _verts;
        
        nNumTextureUnits = _textureUnits;
        
        if (nNumTextureUnits != 0)
        {
            nNumTextureUnits = nNumTextureUnits > 4 ? 4 : nNumTextureUnits;
            vbo_textureCoordArray = new GLuint[nNumTextureUnits];
            pTexCoords = new vec2*[nNumTextureUnits];
            for (int i = 0; i < nNumTextureUnits; i++)
            {
                vbo_textureCoordArray[i] = 0;
                pTexCoords[i] = nullptr;
            }
        }
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }
    
    void Batch::end()
    {

        if (pVerts != nullptr)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexArray);
            glUnmapBuffer(GL_ARRAY_BUFFER);
            pVerts = nullptr;
        }
        if (pNormals != nullptr)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo_normalArray);
            glUnmapBuffer(GL_ARRAY_BUFFER);
            pNormals = nullptr;
        }
        if (pColors != nullptr)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo_colorArray);
            glUnmapBuffer(GL_ARRAY_BUFFER);
            pColors = nullptr;
        }
        
        for(unsigned int i = 0; i < nNumTextureUnits; i++)
            if(pTexCoords[i] != NULL)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoordArray[i]);
                glUnmapBuffer(GL_ARRAY_BUFFER);
                pTexCoords[i] = NULL;
            }
        
        glBindVertexArray(vao);
        
        if (vbo_vertexArray != 0)
        {
            glEnableVertexAttribArray(GLSLProgram::_vertex_att_pos);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexArray);
            glVertexAttribPointer(GLSLProgram::_vertex_att_pos,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE, 0, 0);
        }
        if (vbo_colorArray != 0)
        {
            glEnableVertexAttribArray(GLSLProgram::_vertex_att_color);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_colorArray);
            glVertexAttribPointer(GLSLProgram::_vertex_att_color,
                                  4,
                                  GL_FLOAT,
                                  GL_FALSE, 0, 0);
        }
        if (vbo_normalArray != 0)
        {
            glEnableVertexAttribArray(GLSLProgram::_vertex_att_normal);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_normalArray);
            glVertexAttribPointer(GLSLProgram::_vertex_att_normal,
                                  3,
                                  GL_FLOAT, GL_FALSE, 0, 0);
        }
        
        for(unsigned int i = 0; i < nNumTextureUnits; i++)
            if(vbo_textureCoordArray[i] != 0) {
                glEnableVertexAttribArray(2 + i),
                glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoordArray[i]);
                glVertexAttribPointer(2 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }
        
        finished = true;
        glBindVertexArray(0);
    }
    
    void Batch::vertex3f(GLfloat _x, GLfloat _y, GLfloat _z)
    {
        if (vbo_vertexArray == 0)
        {
            glGenBuffers(1, &vbo_vertexArray);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexArray);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*needToBuild, nullptr, GL_DYNAMIC_DRAW);
        }
        if(pVerts == nullptr)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexArray);
            pVerts = (vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }
        if(builded >= needToBuild) return;
        pVerts[builded][0] = _x;
        pVerts[builded][1] = _y;
        pVerts[builded][2] = _z;
        builded++;
    }
    
    void Batch::vertex3fv(const vec3 &_v)
    {
        vertex3f(_v.x, _v.y, _v.z);
    }
    
    void Batch::normal3f(GLfloat _x, GLfloat _y, GLfloat _z)
    {
        if (vbo_normalArray == 0)
        {
            glGenBuffers(1, &vbo_normalArray);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_normalArray);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*needToBuild, nullptr, GL_DYNAMIC_DRAW);
        }
        if(pNormals == nullptr)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo_normalArray);
            pNormals = (vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }
        if(builded >= needToBuild) return;
        pNormals[builded][0] = _x;
        pNormals[builded][1] = _y;
        pNormals[builded][2] = _z;
    }
    
    void Batch::normal3fv(const vec3 &_v)
    {
        normal3f(_v.x, _v.y, _v.z);
    }
    
    void Batch::color4f(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a)
    {
        if (vbo_colorArray == 0)
        {
            glGenBuffers(1, &vbo_colorArray);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_colorArray);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*needToBuild, nullptr, GL_DYNAMIC_DRAW);
        }
        if(pColors == nullptr)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo_colorArray);
            pColors = (vec4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }
        if(builded >= needToBuild) return;
        pColors[builded][0] = _r;
        pColors[builded][1] = _g;
        pColors[builded][2] = _b;
        pColors[builded][3] = _a;
    }
    
    void Batch::color4fv(const vec4 &_c)
    {
        color4f(_c.r, _c.g, _c.b, _c.a);
    }
    
    void Batch::texCoord2f(GLuint _texture, GLclampf _s, GLclampf _t)
    {
        if (vbo_textureCoordArray[_texture] == 0)
        {
            glGenBuffers(1, &vbo_textureCoordArray[_texture]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoordArray[_texture]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*needToBuild, nullptr, GL_DYNAMIC_DRAW);
        }
        
        if (pTexCoords[_texture] == nullptr)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoordArray[_texture]);
            pTexCoords[_texture] = (vec2*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }
        
        if (builded >= needToBuild) return;
        pTexCoords[_texture][builded][0] = _s;
        pTexCoords[_texture][builded][1] = _t;
    }
    
    void Batch::texCoord2v(GLuint _texture, const vec2 &_tc)
    {
        texCoord2f(_texture, _tc.s, _tc.t);
    }
    
    void Batch::draw()
    {
        if(!finished) return;
        
        glBindVertexArray(vao);
        
        glDrawArrays(primitiveType, 0, needToBuild);
        
        glBindVertexArray(0);
    }
}