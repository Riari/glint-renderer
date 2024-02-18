#include "DirectionalLight.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer
{
    DirectionalLight::DirectionalLight()
        : Light()
        , mDirection(glm::vec3(0.0f, -1.0f, 0.0f))
    {
    }

    DirectionalLight::DirectionalLight(const int shadowWidth, const int shadowHeight, const glm::vec3& color, const float ambientIntensity, const float diffuseIntensity, const glm::vec3& direction)
        : Light(shadowWidth, shadowHeight, color, ambientIntensity, diffuseIntensity)
        , mDirection(direction)
    {
    }

    glm::vec3 DirectionalLight::GetDirection() const
    {
        return mDirection;
    }

    glm::mat4 DirectionalLight::CalculateLightTransform() const
    {
        return mLightProjection * glm::lookAt(-mDirection, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};
