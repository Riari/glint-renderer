#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace World
{
    class Light
    {
    public:
        Light();
        Light(const glm::vec3& colour, const GLfloat intensity);
        ~Light() = default;

        glm::vec3 GetColour() const;
        GLfloat GetIntensity() const;

    private:
        glm::vec3 mColour;
        GLfloat mIntensity;
    };
};
