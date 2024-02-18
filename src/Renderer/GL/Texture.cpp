#include <cassert>

#include <stb_image.h>

#include "Texture.h"

using namespace Renderer::GL;

Texture::Texture(const Asset::Type::Image& image, GLenum unit)
    : Texture(unit)
{
    GenerateFromImage(image);
}

Texture::Texture(GLenum unit)
    : Object()
    , mUnit(unit)
{
    glGenTextures(1, &mId);
    Bind();
}

Texture::~Texture()
{
    glDeleteTextures(1, &mId);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mId);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Generate(GLint width, GLint height, GLenum format, GLenum type, GLint wrapMode, bool withMipmap, const void* data) const
{
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (wrapMode == GL_CLAMP_TO_BORDER)
    {
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    }

    if (withMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

GLenum Texture::GetUnit() const
{
    return mUnit;
}

void Texture::GenerateFromImage(const Asset::Type::Image& image) const
{
    Generate(image.width, image.height, image.channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, GL_REPEAT, true, image.data);
}
