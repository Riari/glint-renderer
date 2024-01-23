#include "Material.h"

namespace World
{
    Material::Material()
        : Material(0.0f, 0.0f)
    {
    }

    Material::Material(GLfloat specularIntensity, GLfloat shininess)
        : mSpecularIntensity(specularIntensity)
        , mShininess(shininess)
    {
    }

    GLfloat Material::GetSpecularIntensity() const
    {
        return mSpecularIntensity;
    }

    GLfloat Material::GetShininess() const
    {
        return mShininess;
    }
};
