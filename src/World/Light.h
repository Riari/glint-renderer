#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace World
{
    class Light
    {
    public:
        Light();
        Light(glm::vec3 colour, GLfloat ambientIntensity);
        ~Light() = default;

        glm::vec3 GetColour() const;
        GLfloat GetAmbientIntensity() const;

    private:
        glm::vec3 mColour;
        GLfloat mAmbientIntensity;
    };
};
