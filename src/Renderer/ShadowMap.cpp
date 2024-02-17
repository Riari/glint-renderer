#include "ShadowMap.h"

namespace Renderer
{
    ShadowMap::ShadowMap(int width, int height)
        : mWidth(width)
        , mHeight(height)
        , mFBO(new GL::FBO())
        , mMap(new GL::Texture(1))
    {
    }

    ShadowMap::~ShadowMap()
    {
        delete mMap;
    }

    bool ShadowMap::Init()
    {
        mMap->Generate(mWidth, mHeight, GL_DEPTH_COMPONENT, nullptr);
        mFBO->AttachTexture(*mMap, GL_DEPTH_ATTACHMENT);
        mFBO->SetDrawMode(GL_NONE);
        mFBO->SetReadMode(GL_NONE);

        bool fboIsValid = mFBO->GetIsValid();
        mFBO->Unbind();

        return fboIsValid;
    }

    GL::Texture* ShadowMap::GetMap() const
    {
        return mMap;
    }
}
