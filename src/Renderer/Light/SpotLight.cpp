#include "SpotLight.h"

namespace Renderer
{
    SpotLight::SpotLight()
        : PointLight()
        , mDirection(glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)))
        , mEdgeAngleDeg(0.0f)
    {
    }

    SpotLight::SpotLight(
        glm::vec3 color,
        float ambientIntensity,
        float diffuseIntensity,
        glm::vec3 position,
        glm::vec3 direction,
        float constant,
        float linear,
        float exponent,
        float edgeAngleDeg)
        : PointLight(color, ambientIntensity, diffuseIntensity, position, constant, linear, exponent)
        , mDirection(glm::normalize(direction))
        , mEdgeAngleDeg(edgeAngleDeg)
        , mEdgeCosine(cosf(glm::radians(edgeAngleDeg)))
    {
    }

    glm::vec3 SpotLight::GetDirection() const
    {
        return mDirection;
    }

    float SpotLight::GetEdgeCosine() const
    {
        return mEdgeCosine;
    }
};
