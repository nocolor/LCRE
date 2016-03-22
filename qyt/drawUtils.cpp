//
// Created by qyt on 16/3/22.
//

#include <GL/glew.h>
#include <GL/glfw3.h>


#include "drawUtils.hpp"
#include "FileUtils.hpp"

namespace qyt
{
    void drawCube(GLSLProgram* program, GLuint textID)
    {
        static GLfloat vertices[] = {
                //top lb
                -1.f,   1.f,    -1.f,           //pos
                1.f,    1.f,    1.f,    1.f,    //color
                0.f,    0.f,                    //uv
                0.f,    1.f,    0.f,            //normal

                //top rb
                1.f,    1.f,    -1.f,           //pos
                1.f,    1.f,    1.f,    1.f,    //color
                1.f,    0.f,                    //uv
                0.f,    1.f,    0.f,            //normal

                //top ru
                1.f,    1.f,    1.f,            //pos
                1.f,    1.f,    1.f,    1.f,    //color
                1.f,    1.f,                    //uv
                0.f,    1.f,    0.f,            //normal

                //top lu
                -1.f,   1.f,    1.f,            //pos
                1.f,    1.f,    1.f,    1.f,    //color
                0.f,    1.f,                    //uv
                0.f,    1.f,    0.f,            //normal

        };

        static GLuint idx[] = {
                //top
                0,  1,  3,
                1,  2,  3
        };

        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint EBO;
        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid * )0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid * )(3* sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid * )(7* sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid * )(9* sizeof(GLfloat)));
        glEnableVertexAttribArray(6);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textID);
        glUniform1i(program->getUniformLocation("__qyt_sampler0"), 0);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

