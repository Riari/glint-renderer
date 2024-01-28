#include <GL/glew.h>
#include <stb_image.h>

#include "Shader.h"
#include "Util/GL.h"

using namespace Renderer::GL;

Shader::Shader(GLuint type)
    : Object(glCreateShader(type))
{
}

Shader::~Shader()
{
    glDeleteShader(mId);
}

int Shader::Build(const std::string& source)
{
    if (mIsBuilt) return 0;

    const char *c_str = source.c_str();
    glShaderSource(mId, 1, &c_str, nullptr);
    glCompileShader(mId);

    if (!Util::GL::CheckShaderBuildStatus(mId, GL_COMPILE_STATUS))
    {
        return -1;
    }

    mIsBuilt = true;

    return 0;
}
