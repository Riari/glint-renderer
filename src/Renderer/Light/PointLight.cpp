#include "PointLight.h"

#include "Renderer/Constants.h"

namespace Renderer
{
    PointLight::PointLight()
        : Light(new ShadowMap(SHADOW_MAP_RESOLUTION, GL_TEXTURE_CUBE_MAP, TEXTURE_UNIT_OMNIDIRECTIONAL_SHADOWMAP))
        , mPosition(glm::vec3(0.0f, 0.0f, 0.0f))
        , mConstant(1.0f)
        , mLinear(0.0f)
        , mExponent(0.0f)
    {
    }

    PointLight::PointLight(
        glm::vec3& color,
        float ambientIntensity,
        float diffuseIntensity,
        glm::vec3& position,
        float constant,
        float linear,
        float exponent)
        : Light(
            color,
            ambientIntensity,
            diffuseIntensity,
            new ShadowMap(SHADOW_MAP_RESOLUTION, GL_TEXTURE_CUBE_MAP, TEXTURE_UNIT_OMNIDIRECTIONAL_SHADOWMAP))
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
