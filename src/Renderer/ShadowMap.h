#pragma once

#include "Renderer/GL/Buffer/FBO.h"
#include "Renderer/GL/Texture.h"

namespace Renderer
{
    class ShadowMap
    {
    public:
        ShadowMap(int size, GLenum textureTarget, int textureUnit);
        ~ShadowMap();

        bool Init();

        void BindFramebuffer();
        void UnbindFramebuffer();

        int GetSize() const;
        GL::Texture* GetMap() const;

    private:
        int mSize;
        GL::FBO* mFBO;
        GL::Texture* mMap;
    };
}
