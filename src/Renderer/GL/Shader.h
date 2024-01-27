#pragma once

#include <GL/glew.h>

#include "Object.h"

namespace Renderer::GL
{
    class Shader : public Object
    {

    public:
        Shader(GLuint type, const char *source);
        ~Shader();

        int Build();

    private:
        bool mIsBuilt{false};
        const char* mSource;
    };
}
