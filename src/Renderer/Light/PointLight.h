#pragma once

#include "Light.h"

namespace Renderer
{
    class PointLight : public Light
    {
    public:
        PointLight();
        PointLight(const glm::vec3& color, const GLfloat ambientIntensity, const GLfloat diffuseIntensity, const glm::vec3& position, const float constant, const float linear, const float exponent);
        ~PointLight() = default;

        glm::vec3 GetPosition() const;
        float GetConstant() const;
        float GetLinear() const;
        float GetExponent() const;

    private:
        glm::vec3 mPosition;
        float mConstant, mLinear, mExponent;
    };
};
