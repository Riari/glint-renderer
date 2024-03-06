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

    void FBO::Bind()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mId);
    }

    void FBO::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::AttachTexture(const GL::Texture& texture, const GLenum attachment) const
    {
        switch (texture.GetTarget())
        {
            case GL_TEXTURE_2D:
                glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.GetID(), 0);
                break;
            case GL_TEXTURE_CUBE_MAP:
                glFramebufferTexture(GL_DRAW_FRAMEBUFFER, attachment, texture.GetID(), 0);
                break;
            default:
                assert(!"Unhandled texture target in FBO::AttachTexture");
        }
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
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
}
