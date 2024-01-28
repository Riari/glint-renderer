#pragma once

#include <string>

#include <GL/glew.h>

#include "Object.h"

namespace Renderer::GL
{
    class Shader : public Object
    {

    public:
        Shader(GLuint type);
        ~Shader();

        int Build(const std::string& source);

    private:
        bool mIsBuilt{false};
        const std::string mSource;
    };
}
