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
        Texture(GLenum unit);
        ~Texture();

        void Bind();
        void Unbind();

        void Generate(GLint width, GLint height, GLenum format, GLenum type, GLint wrapMode, bool withMipmap, const void* data) const;

        GLenum GetUnit() const;

    private:
        GLenum mUnit;

        void GenerateFromImage(const Asset::Type::Image& image) const;
    };
};
