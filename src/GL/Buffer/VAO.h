#pragma once

#include <GL/glew.h>

#include "GL/Object.h"

namespace GL
{
    class VAO : public Object
    {
    public:
        VAO();

        void Bind();
        void Unbind();

        void SetAttributePointer(unsigned int location, unsigned int size, unsigned int stride, const void *pointer) const;
        void EnableAttributePointer(unsigned int location) const;
    };
}
