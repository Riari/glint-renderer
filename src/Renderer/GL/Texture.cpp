#include <cassert>

#include <stb_image.h>

#include "Texture.h"

using namespace Renderer::GL;

Texture::Texture(const Asset::Type::Image& image, GLenum target, int unit)
    : Texture(target, unit)
{
    assert(("Texture unit must be greater than GL_TEXTURE0", unit > 0));
    GenerateFromImage(image);
}

Texture::Texture(GLenum target, int unit)
    : Object()
    , mTarget(target)
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
    glBindTexture(mTarget, mId);
}

void Texture::Unbind()
{
    glBindTexture(mTarget, 0);
}

// TODO: Clean this method up
void Texture::Generate(
    GLint width,
    GLint height,
    GLenum format,
    GLenum type,
    GLint wrapMode,
    bool withMipmap,
    std::vector<void*> data) const
{
    switch (mTarget)
    {
        case GL_TEXTURE_2D:
            glTexImage2D(mTarget, 0, format, width, height, 0, format, type, data[0]);
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, wrapMode);
            glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case GL_TEXTURE_CUBE_MAP:
            for (size_t i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, type, data[i]);
            }

            glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, wrapMode);
            break;
    }

    glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (wrapMode == GL_CLAMP_TO_BORDER)
    {
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(mTarget, GL_TEXTURE_BORDER_COLOR, borderColor);
    }

    if (withMipmap)
    {
        glGenerateMipmap(mTarget);
    }
}

GLenum Texture::GetTarget() const
{
    return mTarget;
}

int Texture::GetUnit() const
{
    return mUnit;
}

void Texture::GenerateFromImage(const Asset::Type::Image& image) const
{
    Generate(image.width, image.height, image.channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, GL_REPEAT, true, {image.data});
}
