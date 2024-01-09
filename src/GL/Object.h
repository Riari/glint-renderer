#pragma once

#include <GL/glew.h>

namespace GL
{
    class Object
    {
    public:
        Object() {}
        Object(GLuint id);

        GLuint GetID() { return mId; }
        bool IsBound() { return mIsBound; }

    protected:
        GLuint mId;
        bool mIsBound{false};
    };
};
