#include "Light.h"

namespace World
{
    Light::Light()
        : Light(glm::vec3(1.0f), 1.0f)
    {
    }

    Light::Light(const glm::vec3& colour, const GLfloat intensity)
        : mColour(colour)
        , mIntensity(intensity)
    {
    }

    glm::vec3 Light::GetColour() const
    {
        return mColour;
    }

    GLfloat Light::GetIntensity() const
    {
        return mIntensity;
    }
};
