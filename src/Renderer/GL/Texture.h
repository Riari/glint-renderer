#pragma once

#include <GL/glew.h>

#include "Object.h"

namespace Renderer::GL
{
    class Texture : public Object
    {
    public:
        Texture(const char* path);
        ~Texture();

        void Bind();
        void Unbind();

    private:
        void Generate(int width, int height, unsigned char* data);
    };
};
