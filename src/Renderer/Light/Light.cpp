#include "Light.h"

#include "Renderer/Constants.h"

namespace Renderer
{
    Light::Light(ShadowMap* shadowMap)
        : Light(glm::vec3(1.0f), 1.0f, 1.0f, shadowMap)
    {
    }

    Light::Light(
        glm::vec3 colour,
        float ambientIntensity,
        float diffuseIntensity,
        ShadowMap* shadowMap)
        : mColour(colour)
        , mAmbientIntensity(ambientIntensity)
        , mDiffuseIntensity(diffuseIntensity)
        , mShadowMap(shadowMap)
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
