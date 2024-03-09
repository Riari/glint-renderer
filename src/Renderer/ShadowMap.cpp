#include "ShadowMap.h"

namespace Renderer
{
    ShadowMap::ShadowMap(int size, GLenum textureTarget, int textureUnit)
        : mSize(size)
        , mFBO(new GL::FBO())
        , mMap(new GL::Texture(textureTarget, textureUnit))
    {
    }

    ShadowMap::~ShadowMap()
    {
        delete mMap;
    }

    bool ShadowMap::Init()
    {
        mMap->Generate(mSize, mSize, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_BORDER, false, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr});
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

    int ShadowMap::GetSize() const
    {
        return mSize;
    }

    GL::Texture* ShadowMap::GetMap() const
    {
        return mMap;
    }
}
