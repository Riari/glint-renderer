#include "DirectionalLight.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Constants.h"

namespace Renderer
{
    DirectionalLight::DirectionalLight()
        : Light(new ShadowMap(SHADOW_MAP_RESOLUTION, GL_TEXTURE_2D, TEXTURE_UNIT_DIRECTIONAL_SHADOWMAP))
        , mDirection(glm::vec3(0.0f, -1.0f, 0.0f))
    {
    }

    DirectionalLight::DirectionalLight(
        glm::vec3 color,
        float ambientIntensity,
        float diffuseIntensity,
        glm::vec3 direction)
        : Light(
            color,
            ambientIntensity,
            diffuseIntensity,
            new ShadowMap(SHADOW_MAP_RESOLUTION, GL_TEXTURE_2D, TEXTURE_UNIT_DIRECTIONAL_SHADOWMAP))
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
