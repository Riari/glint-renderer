#include <GL/glew.h>

#include "Shader.h"
#include "Util/GL.h"

using namespace Renderer::GL;

Shader::Shader(GLuint type, const char *source) : mSource(source), Object(glCreateShader(type))
{
}

Shader::~Shader()
{
    glDeleteShader(mId);
}

int Shader::Build()
{
    if (mIsBuilt) return 0;

    glShaderSource(mId, 1, &mSource, nullptr);
    glCompileShader(mId);

    if (!Util::GL::CheckShaderBuildStatus(mId, GL_COMPILE_STATUS))
    {
        return -1;
    }

    mIsBuilt = true;

    return 0;
}
