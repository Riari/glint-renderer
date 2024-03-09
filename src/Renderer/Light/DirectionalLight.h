#pragma once

#include "Light.h"

namespace Renderer
{
    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(
            glm::vec3 color,
            float ambientIntensity,
            float diffuseIntensity,
            glm::vec3 direction,
            int shadowMapUnit);
        ~DirectionalLight() = default;

        void SetDirection(glm::vec3 direction);
        glm::vec3 GetDirection() const;

        glm::mat4 CalculateLightTransform() const;

    private:
        glm::vec3 mDirection;
    };
};
