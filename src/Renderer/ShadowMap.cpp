#include "ShadowMap.h"

namespace Renderer
{
    ShadowMap::ShadowMap(int width, int height)
        : mWidth(width)
        , mHeight(height)
        , mFBO(new GL::FBO())
        , mMap(new GL::Texture(5)) // TODO: Don't hardcode the texture unit
    {
    }

    ShadowMap::~ShadowMap()
    {
        delete mMap;
    }

    bool ShadowMap::Init()
    {
        mMap->Generate(mWidth, mHeight, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_BORDER, false, nullptr);
        mFBO->Bind();
        mFBO->AttachTexture(*mMap, GL_DEPTH_ATTACHMENT);
        mFBO->SetDrawMode(GL_NONE);
        mFBO->SetReadMode(GL_NONE);

        bool fboIsValid = mFBO->GetIsValid();
        mFBO->Unbind();

        return fboIsValid;
    }

    void ShadowMap::BindFramebuffer()
    {
        mFBO->Bind();
    }

    void ShadowMap::UnbindFramebuffer()
    {
        mFBO->Unbind();
    }

    int ShadowMap::GetWidth() const
    {
        return mWidth;
    }

    int ShadowMap::GetHeight() const
    {
        return mHeight;
    }

    GL::Texture* ShadowMap::GetMap() const
    {
        return mMap;
    }
}
