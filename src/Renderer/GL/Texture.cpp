#include <cassert>

#include <stb_image.h>

#include "Texture.h"

using namespace Renderer::GL;

Texture::Texture(const Asset::Type::Image& image) : Object()
{
    glGenTextures(1, &mId);

    Bind();
    GenerateFromImage(image);
}

Texture::~Texture()
{
    glDeleteTextures(1, &mId);
}

void Texture::GenerateFromImage(const Asset::Type::Image& image)
{
    assert(("Texture must be bound before operating on it", mIsBound));

    auto format = image.channels == 3 ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind()
{
    if (mIsBound) return;

    glBindTexture(GL_TEXTURE_2D, mId);
    mIsBound = true;
}

void Texture::Unbind()
{
    if (!mIsBound) return;

    glBindTexture(GL_TEXTURE_2D, 0);
    mIsBound = false;
}
