#pragma once

#include <glm/glm.hpp>

#include "GL/Texture.h"

namespace Renderer
{
    enum MapType
    {
        TEXTURE,
        COLOUR
    };

    class Map
    {
    public:
        Map(glm::vec3 colour);
        Map(const char* texturePath);
        ~Map();

        MapType GetType() const;
        glm::vec3 GetColour() const;
        GL::Texture* GetTexture() const;

        void Bind() const;
        void Unbind() const;

    private:
        MapType mType;
        glm::vec3 mColour;
        GL::Texture* mTexture;
    };
};
