#pragma once

#include <GL/glew.h>

#include "Map.h"

namespace Renderer
{
    // TODO: Validate materials. They should have at least a diffuse map.
    class Material
    {
    public:
        Material();
        Material(GLfloat specularIntensity, GLfloat shininess);
        ~Material();

        /// @brief Sets the provided map according to its target type.
        /// CAUTION: This will replace any existing map with the same target type!
        void SetMap(Map* map);

        Map* GetMap(Asset::Type::MapTargetType targetType) const;
        GLfloat GetSpecularIntensity() const;
        GLfloat GetShininess() const;

        void Bind() const;
        void Unbind() const;

    private:
        std::unordered_map<Asset::Type::MapTargetType, Map*> mMaps;

        GLfloat mSpecularIntensity;
        GLfloat mShininess;
    };
};
