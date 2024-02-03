#pragma once

#include <glm/glm.hpp>

#include "Asset/Type/Image.h"
#include "Asset/Type/Model.h"
#include "Renderer/GL/Texture.h"

namespace Renderer
{
    /// @brief Represents a solid colour or texture map.
    class Map
    {
    public:
        Map(const Asset::Type::Image& image, Asset::Type::MapTargetType targetType);
        Map(glm::vec3 colour, Asset::Type::MapTargetType targetType);
        ~Map();

        Asset::Type::MapSourceType GetSourceType() const;
        Asset::Type::MapTargetType GetTargetType() const;
        glm::vec3 GetColour() const;
        GL::Texture* GetTexture() const;

        GLenum GetTextureUnit() const;

        void Bind() const;
        void Unbind() const;

    private:
        Asset::Type::MapSourceType mSourceType;
        Asset::Type::MapTargetType mTargetType;
        glm::vec3 mColour;
        GL::Texture* mTexture;
        GLenum mTextureUnit;
    };
};
