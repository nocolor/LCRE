//
// Created by qyt on 16/3/25.
//

#ifndef OPENGL_GLRENDERCOMMAND_HPP
#define OPENGL_GLRENDERCOMMAND_HPP

#include <stdint.h>
#include "math/math.hpp"
#include <OpenGL/gl.h>

namespace qyt
{
    class GLRenderCommand
    {
        enum class RenderCommandType
        {
            unkown,
            quad,
            custom,
            group,
            triangle
        };

        inline void init(float order) { _order = order; }
        inline float getOrder() const { return _isTransparent; }
        inline RenderCommandType getType() const { return _type; }
    protected:
        GLRenderCommand();
        virtual  ~GLRenderCommand();
        RenderCommandType _Type;

        float _order;
        bool _isTransparent;
        RenderCommandType _type;
    };

    //************************************************
    class GLQuadCommand : public GLRenderCommand
    {
    protected:
        GLuint _textureID;
        mat4 _mv;
    };
}
#endif //OPENGL_GLRENDERCOMMAND_HPP
