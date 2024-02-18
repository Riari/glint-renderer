#pragma once

#include "Light.h"

namespace Renderer
{
    class DirectionalLight : public Light
    {
    public:
        DirectionalLight();
        DirectionalLight(const int shadowWidth, const int shadowHeight, const glm::vec3& color, const float ambientIntensity, const float diffuseIntensity, const glm::vec3& direction);
        ~DirectionalLight() = default;

        glm::vec3 GetDirection() const;

        glm::mat4 CalculateLightTransform() const;

    private:
        glm::vec3 mDirection;
    };
};
