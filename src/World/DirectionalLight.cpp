#include "DirectionalLight.h"

namespace World
{
    DirectionalLight::DirectionalLight()
        : Light()
        , mDirection(glm::vec3(0.0f, -1.0f, 0.0f))
    {
    }

    DirectionalLight::DirectionalLight(const glm::vec3& color, const GLfloat intensity, const glm::vec3& direction)
        : Light(color, intensity)
        , mDirection(direction)
    {
    }

    glm::vec3 DirectionalLight::GetDirection() const
    {
        return mDirection;
    }
};
