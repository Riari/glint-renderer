#pragma once

#include <GL/glew.h>

namespace Renderer::GL
{
    class Object
    {
    public:
        Object() {}
        Object(GLuint id);

        GLuint GetID() const { return mId; }
        bool IsBound() const { return mIsBound; }

    protected:
        GLuint mId;
        bool mIsBound{false};
    };
};
