#include "ShaderProgram.h"

#include "Util/GL.h"

using namespace Renderer::GL;

ShaderProgram::ShaderProgram() :
    mVertex(new Shader(GL_VERTEX_SHADER)),
    mFragment(new Shader(GL_FRAGMENT_SHADER)),
    Object(glCreateProgram())
{
}

int ShaderProgram::Build(const std::string& vertexSource, const std::string& fragmentSource)
{
    if (mIsBuilt) return 0;

    mVertex->Build(vertexSource);
    mFragment->Build(fragmentSource);

    glAttachShader(mId, mVertex->GetID());
    glAttachShader(mId, mFragment->GetID());
    glLinkProgram(mId);

    if (!Util::GL::CheckShaderBuildStatus(mId, GL_LINK_STATUS))
    {
        return -1;
    }

    mIsBuilt = true;
    delete mVertex;
    delete mFragment;

    return 0;
}

void ShaderProgram::Use() const
{
    glUseProgram(mId);
}

void ShaderProgram::SetUniform1i(std::string name, const int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform1f(std::string name, const float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform3f(std::string name, const glm::vec3 value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void ShaderProgram::SetUniform4fv(std::string name, const float* pValue)
{
    glUniform4fv(GetUniformLocation(name), 1, pValue);
}

void ShaderProgram::SetUniformMatrix4fv(std::string name, const float* pValue)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, pValue);
}

int ShaderProgram::GetUniformLocation(std::string& name)
{
    auto it = mUniforms.find(name);
    if (it == mUniforms.end())
    {
        mUniforms[name] = glGetUniformLocation(mId, name.c_str());
        return mUniforms[name];
    }

    return it->second;
}
