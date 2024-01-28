#include "Map.h"

namespace Renderer
{
    Map::Map(glm::vec3 colour)
        : mType(MapType::COLOUR)
        , mColour(colour)
    {
    }

    Map::Map(const Asset::Type::Image& image)
        : mType(MapType::TEXTURE)
        , mTexture(new GL::Texture(image))
    {
    }

    Map::~Map()
    {
        if (mType == MapType::TEXTURE)
        {
            delete mTexture;
        }
    }

    MapType Map::GetType() const
    {
        return mType;
    }

    glm::vec3 Map::GetColour() const
    {
        return mColour;
    }

    GL::Texture* Map::GetTexture() const
    {
        return mTexture;
    }

    void Map::Bind() const
    {
        if (mType != MapType::TEXTURE) return;

        mTexture->Bind();
    }

    void Map::Unbind() const
    {
        if (mType != MapType::TEXTURE) return;

        mTexture->Unbind();
    }
};
