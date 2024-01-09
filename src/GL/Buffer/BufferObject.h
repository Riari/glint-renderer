#pragma once

#include <GL/glew.h>

#include "GL/Object.h"

namespace GL
{
    class BufferObject : public Object
    {
    public:
        BufferObject(GLenum target);
        ~BufferObject();

        void Bind();
        void Unbind();

        void Write(unsigned int size, const void *pData, const unsigned int usage) const;

        GLenum GetTarget() const { return mTarget; }

    protected:
        GLenum mTarget;
    };
}
