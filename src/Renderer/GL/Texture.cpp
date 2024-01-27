#include <cassert>

#include <stb_image.h>

#include "Texture.h"

using namespace Renderer::GL;

Texture::Texture(const char* path) : Object()
{
    glGenTextures(1, &mId);

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    Bind();
    Generate(width, height, data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &mId);
}

void Texture::Generate(int width, int height, unsigned char* data)
{
    assert(("Texture must be bound before operating on it", mIsBound));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
