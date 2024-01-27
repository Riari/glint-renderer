#include "Material.h"

namespace Renderer
{
    Material::Material()
        : Material(new Map(glm::vec3(1.0f, 1.0f, 1.0f)), 0.0f, 0.0f)
    {
    }

    Material::Material(Map* baseMap, GLfloat specularIntensity, GLfloat shininess)
        : mBaseMap(baseMap)
        , mSpecularIntensity(specularIntensity)
        , mShininess(shininess)
    {
    }

    Material::~Material()
    {
        delete mBaseMap;
    }

    Map* Material::GetBaseMap() const
    {
        return mBaseMap;
    }

    GLfloat Material::GetSpecularIntensity() const
    {
        return mSpecularIntensity;
    }

    GLfloat Material::GetShininess() const
    {
        return mShininess;
    }

    void Material::Bind() const
    {
        mBaseMap->Bind();
    }

    void Material::Unbind() const
    {
        mBaseMap->Unbind();
    }
};
