#include <cassert>

#include "BufferObject.h"

using namespace Renderer::GL;

BufferObject::BufferObject(GLenum target)
    : Object()
    , mTarget(target)
{
    glGenBuffers(1, &mId);
}

BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &mId);
}

void BufferObject::Bind()
{
    glBindBuffer(mTarget, mId);
}

void BufferObject::Unbind()
{
    glBindBuffer(mTarget, 0);
}

void BufferObject::Write(unsigned int size, const void *pData, const unsigned int usage) const
{
    glBufferData(mTarget, size, pData, usage);
}
