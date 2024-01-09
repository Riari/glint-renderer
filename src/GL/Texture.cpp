#include <cassert>

#include "Texture.h"

using namespace GL;

Texture::Texture() : Object() {
    glGenTextures(1, &mId);
}

Texture::~Texture() {
    glDeleteTextures(1, &mId);
}

void Texture::Generate(int width, int height, unsigned char* data) {
    assert(("Texture must be bound before operating on it", mIsBound));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, mId);
    mIsBound = true;
}

void Texture::Unbind() {
    if (!mIsBound) return;

    glBindTexture(GL_TEXTURE_2D, 0);
    mIsBound = false;
}
