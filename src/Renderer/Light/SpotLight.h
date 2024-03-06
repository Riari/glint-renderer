#pragma once

#include "PointLight.h"

namespace Renderer
{
    class SpotLight : public PointLight
    {
    public:
        SpotLight(
            glm::vec3 color,
            float ambientIntensity,
            float diffuseIntensity,
            glm::vec3 position,
            glm::vec3 direction,
            float constant,
            float linear,
            float exponent,
            float edge);
        ~SpotLight() = default;

        glm::vec3 GetDirection() const;
        float GetEdgeCosine() const;

    private:
        glm::vec3 mDirection;
        float mEdgeAngleDeg, mEdgeCosine;
    };
};
