#include "Light.h"

namespace Renderer
{
    Light::Light()
        : Light(2048, 2048, glm::vec3(1.0f), 1.0f, 1.0f)
    {
    }

    Light::Light(const int shadowWidth, const int shadowHeight, const glm::vec3& colour, const float ambientIntensity, const float diffuseIntensity)
        : mColour(colour)
        , mAmbientIntensity(ambientIntensity)
        , mDiffuseIntensity(diffuseIntensity)
        , mShadowMap(new ShadowMap(shadowWidth, shadowHeight))
    {
        // TODO: Handle init failure
        mShadowMap->Init();
    }

    Light::~Light()
    {
        delete mShadowMap;
    }

    glm::vec3 Light::GetColour() const
    {
        return mColour;
    }

    float Light::GetAmbientIntensity() const
    {
        return mAmbientIntensity;
    }

    float Light::GetDiffuseIntensity() const
    {
        return mDiffuseIntensity;
    }

    ShadowMap* Light::GetShadowMap() const
    {
        return mShadowMap;
    }
};
