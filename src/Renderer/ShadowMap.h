#pragma once

#include "Renderer/GL/Buffer/FBO.h"
#include "Renderer/GL/Texture.h"

namespace Renderer
{
    class ShadowMap
    {
    public:
        ShadowMap(int width, int height);
        ~ShadowMap();

        bool Init();

        void BindFramebuffer();
        void UnbindFramebuffer();

        int GetWidth() const;
        int GetHeight() const;
        GL::Texture* GetMap() const;

    private:
        int mWidth, mHeight;
        GL::FBO* mFBO;
        GL::Texture* mMap;
    };
}
