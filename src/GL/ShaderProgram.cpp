#include "ShaderProgram.h"

#include "Util/GL.h"

using namespace GL;
using namespace Util::GL;

ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource) :
    pVertex(new Shader(GL_VERTEX_SHADER, vertexSource)),
    pFragment(new Shader(GL_FRAGMENT_SHADER, fragmentSource)),
    Object(glCreateProgram())
{
}

int ShaderProgram::Build()
{
    if (mIsBuilt) return 0;

    pVertex->Build();
    pFragment->Build();

    glAttachShader(mId, pVertex->GetID());
    glAttachShader(mId, pFragment->GetID());
    glLinkProgram(mId);

    if (!CheckShaderBuildStatus(mId, GL_LINK_STATUS))
    {
        return -1;
    }

    mIsBuilt = true;
    delete pVertex;
    delete pFragment;

    return 0;
}

void ShaderProgram::Use() const
{
    glUseProgram(mId);
}

void ShaderProgram::SetUniform1i(const char* pName, const int value) const
{
    glUniform1i(glGetUniformLocation(mId, pName), value);
}

void ShaderProgram::SetUniform1f(const char* pName, const float value) const
{
    glUniform1f(glGetUniformLocation(mId, pName), value);
}

void ShaderProgram::SetUniform3f(const char* pName, const glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(mId, pName), value.x, value.y, value.z);
}

void ShaderProgram::SetUniform4fv(const char* pName, const float* pValue) const
{
    glUniform4fv(glGetUniformLocation(mId, pName), 1, pValue);
}

void ShaderProgram::SetUniformMatrix4fv(const char* pName, const float* pValue) const
{
    glUniformMatrix4fv(glGetUniformLocation(mId, pName), 1, GL_FALSE, pValue);
}
