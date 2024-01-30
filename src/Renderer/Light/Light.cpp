#include "Light.h"

namespace Renderer
{
    Light::Light()
        : Light(glm::vec3(1.0f), 1.0f, 1.0f)
    {
    }

    Light::Light(const glm::vec3& colour, const float ambientIntensity, const float diffuseIntensity)
        : mColour(colour)
        , mAmbientIntensity(ambientIntensity)
        , mDiffuseIntensity(diffuseIntensity)
    {
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
};
