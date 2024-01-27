#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace World
{
    class Light
    {
    public:
        Light();
        Light(const glm::vec3& colour, const GLfloat ambientIntensity, const GLfloat diffuseIntensity);
        ~Light() = default;

        glm::vec3 GetColour() const;
        GLfloat GetAmbientIntensity() const;
        GLfloat GetDiffuseIntensity() const;

    private:
        glm::vec3 mColour;
        GLfloat mAmbientIntensity;
        GLfloat mDiffuseIntensity;
    };
};
