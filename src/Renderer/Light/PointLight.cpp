#include "PointLight.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Constants.h"
#include "Renderer/OmniShadowMap.h"

namespace Renderer
{
    PointLight::PointLight(
        glm::vec3 color,
        float ambientIntensity,
        float diffuseIntensity,
        glm::vec3 position,
        float constant,
        float linear,
        float exponent,
        int shadowMapUnit)
        : Light(
            color,
            ambientIntensity,
            diffuseIntensity,
            new OmniShadowMap(SHADOW_MAP_RESOLUTION, shadowMapUnit))
        , mPosition(position)
        , mConstant(constant)
        , mLinear(linear)
        , mExponent(exponent)
    {
        float aspect = (float)SHADOW_MAP_RESOLUTION / (float)SHADOW_MAP_RESOLUTION;
        mProjection = glm::perspective(glm::radians(90.0f), aspect, NEAR_PLANE, FAR_PLANE);
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

    std::vector<glm::mat4> PointLight::CalculateLightTransforms() const
    {
        std::vector<glm::mat4> matrices;

        // +x, -x
        matrices.push_back(mProjection * glm::lookAt(mPosition, mPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        matrices.push_back(mProjection * glm::lookAt(mPosition, mPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        // +y, -y
        matrices.push_back(mProjection * glm::lookAt(mPosition, mPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        matrices.push_back(mProjection * glm::lookAt(mPosition, mPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));

        // +z, -z
        matrices.push_back(mProjection * glm::lookAt(mPosition, mPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        matrices.push_back(mProjection * glm::lookAt(mPosition, mPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        return matrices;
    }
};
