#include "ShaderProgram.h"

#include "Util/GL.h"

using namespace GL;

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

    if (!Util::GL::CheckShaderBuildStatus(mId, GL_LINK_STATUS))
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

void ShaderProgram::SetUniform1i(const char* pName, const int value)
{
    glUniform1i(GetUniformLocation(pName), value);
}

void ShaderProgram::SetUniform1f(const char* pName, const float value)
{
    glUniform1f(GetUniformLocation(pName), value);
}

void ShaderProgram::SetUniform3f(const char* pName, const glm::vec3 value)
{
    glUniform3f(GetUniformLocation(pName), value.x, value.y, value.z);
}

void ShaderProgram::SetUniform4fv(const char* pName, const float* pValue)
{
    glUniform4fv(GetUniformLocation(pName), 1, pValue);
}

void ShaderProgram::SetUniformMatrix4fv(const char* pName, const float* pValue)
{
    glUniformMatrix4fv(GetUniformLocation(pName), 1, GL_FALSE, pValue);
}

int ShaderProgram::GetUniformLocation(const char* pName)
{
    auto it = mUniforms.find(pName);
    if (it == mUniforms.end())
    {
        mUniforms[pName] = glGetUniformLocation(mId, pName);
        return mUniforms[pName];
    }

    return it->second;
}
