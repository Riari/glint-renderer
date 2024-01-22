#pragma once

#include "Light.h"

namespace World
{
    class DirectionalLight : public Light
    {
    public:
        DirectionalLight();
        DirectionalLight(const glm::vec3& color, const GLfloat intensity, const glm::vec3& direction);
        ~DirectionalLight() = default;

        glm::vec3 GetDirection() const;

    private:
        glm::vec3 mDirection;
    };
};
