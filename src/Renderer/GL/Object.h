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

    protected:
        GLuint mId;
        bool mIsBound{false};
    };
};
