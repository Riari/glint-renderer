#pragma once

#include "Renderer/GL/Object.h"
#include "Renderer/GL/Texture.h"

namespace Renderer::GL
{
    class FBO : Object
    {
    public:
        FBO();
        ~FBO();

        void Bind() const;
        void Unbind() const;

        void AttachTexture(const GL::Texture& texture, const GLenum attachment) const;

        void SetDrawMode(GLenum mode) const;
        void SetReadMode(GLenum mode) const;

        bool GetIsValid() const;
    };
}
