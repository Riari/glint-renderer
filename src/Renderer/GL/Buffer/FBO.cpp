#include "FBO.h"

#include <cassert>

namespace Renderer::GL
{
    FBO::FBO()
        : Object()
    {
        glGenFramebuffers(1, &mId);
    }

    FBO::~FBO()
    {
        glDeleteFramebuffers(1, &mId);
    }

    void FBO::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mId);
    }

    void FBO::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::AttachTexture(const GL::Texture& texture, const GLenum attachment) const
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.GetID(), 0);
    }

    void FBO::SetDrawMode(GLenum mode) const
    {
        glDrawBuffer(mode);
    }

    void FBO::SetReadMode(GLenum mode) const
    {
        glReadBuffer(mode);
    }

    bool FBO::GetIsValid() const
    {
        assert(("FBO must be bound before checking if it's valid", mIsBound));

        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
}
