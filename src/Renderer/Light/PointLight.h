#pragma once

#include <vector>

#include "Light.h"

namespace Renderer
{
    class PointLight : public Light
    {
    public:
        PointLight(
            glm::vec3 color,
            float ambientIntensity,
            float diffuseIntensity,
            glm::vec3 position,
            float constant,
            float linear,
            float exponent,
            int shadowMapUnit);
        ~PointLight() = default;

        glm::vec3 GetPosition() const;
        float GetConstant() const;
        float GetLinear() const;
        float GetExponent() const;

        std::vector<glm::mat4> CalculateLightTransforms() const;

    protected:
        glm::vec3 mPosition;
        float mConstant, mLinear, mExponent;
    };
};
