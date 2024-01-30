#pragma once

#include <GL/glew.h>

#include "Asset/Type/Image.h"
#include "Object.h"

namespace Renderer::GL
{
    class Texture : public Object
    {
    public:
        Texture(const Asset::Type::Image& image);
        ~Texture();

        void Bind();
        void Unbind();

    private:
        void GenerateFromImage(const Asset::Type::Image& image);
    };
};
