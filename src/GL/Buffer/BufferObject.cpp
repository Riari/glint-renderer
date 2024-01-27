#include <cassert>

#include "BufferObject.h"

using namespace GL;

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
    if (mIsBound) return;

    glBindBuffer(mTarget, mId);
    mIsBound = true;
}

void BufferObject::Unbind()
{
    if (!mIsBound) return;

    glBindBuffer(mTarget, 0);
    mIsBound = false;
}

void BufferObject::Write(unsigned int size, const void *pData, const unsigned int usage) const
{
    glBufferData(mTarget, size, pData, usage);
}
