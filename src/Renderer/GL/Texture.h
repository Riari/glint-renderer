#pragma once

#include <GL/glew.h>

#include "Asset/Type/Image.h"
#include "Object.h"

namespace Renderer::GL
{
    class Texture : public Object
    {
    public:
        Texture(const Asset::Type::Image& image, GLenum unit);
        ~Texture();

        void Bind();
        void Unbind();

    private:
        GLenum mUnit;

        void GenerateFromImage(const Asset::Type::Image& image);
    };
};
