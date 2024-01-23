#pragma once

#include <GL/glew.h>

namespace World
{
    class Material
    {
    public:
        Material();
        Material(GLfloat specularIntensity, GLfloat shininess);
        ~Material() = default;

        GLfloat GetSpecularIntensity() const;
        GLfloat GetShininess() const;

    private:
        GLfloat mSpecularIntensity;
        GLfloat mShininess;
    };
};
