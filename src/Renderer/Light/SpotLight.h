#pragma once

#include "PointLight.h"

namespace Renderer
{
    class SpotLight : public PointLight
    {
    public:
        SpotLight();
        SpotLight(
            const glm::vec3& color,
            const float ambientIntensity,
            const float diffuseIntensity,
            const glm::vec3& position,
            const glm::vec3& direction,
            const float constant,
            const float linear,
            const float exponent,
            const float edge);
        ~SpotLight() = default;

        glm::vec3 GetDirection() const;
        float GetEdgeCosine() const;

    private:
        glm::vec3 mDirection;

        float mEdgeAngleDeg, mEdgeCosine;
    };
};
