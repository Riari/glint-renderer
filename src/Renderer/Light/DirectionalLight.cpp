#include "DirectionalLight.h"

namespace Renderer
{
    DirectionalLight::DirectionalLight()
        : Light()
        , mDirection(glm::vec3(0.0f, -1.0f, 0.0f))
    {
    }

    DirectionalLight::DirectionalLight(const glm::vec3& color, const float ambientIntensity, const float diffuseIntensity, const glm::vec3& direction)
        : Light(color, ambientIntensity, diffuseIntensity)
        , mDirection(direction)
    {
    }

    glm::vec3 DirectionalLight::GetDirection() const
    {
        return mDirection;
    }
};
