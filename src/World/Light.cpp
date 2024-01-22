#include "Light.h"

namespace World
{
    Light::Light()
        : Light(glm::vec3(1.0f), 1.0f)
    {
    }

    Light::Light(glm::vec3 colour, GLfloat ambientIntensity)
        : mColour(colour)
        , mAmbientIntensity(ambientIntensity)
    {
    }

    glm::vec3 Light::GetColour() const
    {
        return mColour;
    }

    GLfloat Light::GetAmbientIntensity() const
    {
        return mAmbientIntensity;
    }
};
