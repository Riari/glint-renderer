#include "Material.h"

namespace Renderer
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

    Material::~Material()
    {
        for (auto& map : mMaps)
        {
            delete map.second;
        }
        mMaps.clear();
    }

    void Material::SetMap(Map* map)
    {
        // TODO: This should probably delete the old map if there is one
        mMaps[map->GetTargetType()] = map;
    }

    Map* Material::GetMap(Asset::Type::MapTargetType targetType) const
    {
        auto it = mMaps.find(targetType);
        if (it != mMaps.end())
        {
            return it->second;
        }

        return nullptr;
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
        for (auto& map : mMaps)
        {
            map.second->Bind();
        }
    }

    void Material::Unbind() const
    {
        for (auto& map : mMaps)
        {
            map.second->Unbind();
        }
    }
};
