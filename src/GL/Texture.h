#pragma once

#include <GL/glew.h>

#include "Object.h"

namespace GL
{
    class Texture : public Object
    {
    public:
        Texture();
        ~Texture();

        void Generate(int width, int height, unsigned char* data);
        void Bind();
        void Unbind();
    };
};
