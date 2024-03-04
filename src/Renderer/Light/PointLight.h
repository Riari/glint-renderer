#pragma once

#include "Light.h"

namespace Renderer
{
    class PointLight : public Light
    {
    public:
        PointLight();
        PointLight(
            glm::vec3& color,
            float ambientIntensity,
            float diffuseIntensity,
            glm::vec3& position,
            float constant,
            float linear,
            float exponent);
        ~PointLight() = default;

        glm::vec3 GetPosition() const;
        float GetConstant() const;
        float GetLinear() const;
        float GetExponent() const;

    protected:
        glm::vec3 mPosition;
        float mConstant, mLinear, mExponent;
    };
};
