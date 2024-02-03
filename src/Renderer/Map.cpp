#include "Map.h"

namespace Renderer
{
    Map::Map(glm::vec3 colour, Asset::Type::MapTargetType targetType)
        : mSourceType(Asset::Type::MapSourceType::COLOUR)
        , mTargetType(targetType)
        , mColour(colour)
    {
    }

    Map::Map(const Asset::Type::Image& image, Asset::Type::MapTargetType targetType)
        : mSourceType(Asset::Type::MapSourceType::IMAGE)
        , mTargetType(targetType)
    {
        switch (mTargetType)
        {
            case Asset::Type::MapTargetType::DIFFUSE:
                mTextureUnit = 0;
                break;
            case Asset::Type::MapTargetType::SPECULAR:
                mTextureUnit = 1;
                break;
        }

        mTexture = new GL::Texture(image, mTextureUnit);
    }

    Map::~Map()
    {
        if (mSourceType == Asset::Type::MapSourceType::IMAGE)
        {
            delete mTexture;
        }
    }

    Asset::Type::MapSourceType Map::GetSourceType() const
    {
        return mSourceType;
    }

    Asset::Type::MapTargetType Map::GetTargetType() const
    {
        return mTargetType;
    }

    glm::vec3 Map::GetColour() const
    {
        return mColour;
    }

    GL::Texture* Map::GetTexture() const
    {
        return mTexture;
    }

    GLuint Map::GetTextureUnit() const
    {
        return mTextureUnit;
    }

    void Map::Bind() const
    {
        if (mSourceType != Asset::Type::MapSourceType::IMAGE) return;

        mTexture->Bind();
    }

    void Map::Unbind() const
    {
        if (mSourceType != Asset::Type::MapSourceType::IMAGE) return;

        mTexture->Unbind();
    }
};
