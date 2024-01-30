#include "PointLight.h"

namespace Renderer
{
    PointLight::PointLight()
        : Light()
        , mPosition(glm::vec3(0.0f, 0.0f, 0.0f))
        , mConstant(1.0f)
        , mLinear(0.0f)
        , mExponent(0.0f)
    {
    }

    PointLight::PointLight(
        const glm::vec3& color,
        const float ambientIntensity,
        const float diffuseIntensity,
        const glm::vec3& position,
        const float constant,
        const float linear,
        const float exponent)
        : Light(color, ambientIntensity, diffuseIntensity)
        , mPosition(position)
        , mConstant(constant)
        , mLinear(linear)
        , mExponent(exponent)
    {
    }

    glm::vec3 PointLight::GetPosition() const
    {
        return mPosition;
    }

    float PointLight::GetConstant() const
    {
        return mConstant;
    }

    float PointLight::GetLinear() const
    {
        return mLinear;
    }

    float PointLight::GetExponent() const
    {
        return mExponent;
    }
};
