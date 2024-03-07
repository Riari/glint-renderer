#include "DirectionalLight.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Constants.h"

namespace Renderer
{
    DirectionalLight::DirectionalLight(
        glm::vec3 color,
        float ambientIntensity,
        float diffuseIntensity,
        glm::vec3 direction,
        int shadowMapUnit)
        : Light(
            color,
            ambientIntensity,
            diffuseIntensity,
            new ShadowMap(SHADOW_MAP_RESOLUTION, GL_TEXTURE_2D, shadowMapUnit))
        , mDirection(direction)
    {
        mProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
    }

    glm::vec3 DirectionalLight::GetDirection() const
    {
        return mDirection;
    }

    glm::mat4 DirectionalLight::CalculateLightTransform() const
    {
        return mProjection * glm::lookAt(-mDirection, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};
