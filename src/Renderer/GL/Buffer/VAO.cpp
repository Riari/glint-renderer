#include <cassert>

#include "VAO.h"

using namespace Renderer::GL;

VAO::VAO() : Object()
{
    glGenVertexArrays(1, &mId);
}

void VAO::Bind()
{
    if (mIsBound) return;

    glBindVertexArray(mId);
    mIsBound = true;
}

void VAO::Unbind()
{
    if (!mIsBound) return;

    glBindVertexArray(0);
    mIsBound = false;
}

void VAO::SetAttributePointer(unsigned int location, unsigned int size, unsigned int stride, const void *pointer) const
{
    glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, pointer);
}

void VAO::EnableAttributePointer(unsigned int location) const
{
    glEnableVertexAttribArray(location);
}
