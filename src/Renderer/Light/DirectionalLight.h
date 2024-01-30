#pragma once

#include "Light.h"

namespace Renderer
{
    class DirectionalLight : public Light
    {
    public:
        DirectionalLight();
        DirectionalLight(const glm::vec3& color, const float ambientIntensity, const float diffuseIntensity, const glm::vec3& direction);
        ~DirectionalLight() = default;

        glm::vec3 GetDirection() const;

    private:
        glm::vec3 mDirection;
    };
};
