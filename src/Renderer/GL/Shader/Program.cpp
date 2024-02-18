#include "Program.h"

#include <spdlog/spdlog.h>

#include "Util/GL.h"

using namespace Renderer::GL::Shader;

Program::Program()
    : Object(glCreateProgram())
{
}

Program::~Program()
{
    glDeleteProgram(mId);
}

// TODO: Improve validation (ensure required shader types are present, warn when overwriting an existing shader, etc).
int Program::AddShader(const GLuint shaderType, const std::string& source)
{
    int compileStatus = CompileShader(shaderType, source);
    if (compileStatus != 0)
    {
        return compileStatus;
    }

    mShaderTypes.push_back(shaderType);

    return 0;
}

int Program::Link()
{
    if (mShaderTypes.empty()) return -1;

    glLinkProgram(mId);
    if (!CheckProgramStatus(GL_LINK_STATUS))
    {
        return -1;
    }

    glValidateProgram(mId);
    if (!CheckProgramStatus(GL_VALIDATE_STATUS))
    {
        return -1;
    }

    mIsBuilt = true;

    return 0;
}

void Program::Use() const
{
    glUseProgram(mId);
}

void Program::SetUniform1i(std::string name, const int value)
{
    int location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void Program::SetUniform1f(std::string name, const float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Program::SetUniform3f(std::string name, const glm::vec3& value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Program::SetUniform4fv(std::string name, const float* pValue)
{
    glUniform4fv(GetUniformLocation(name), 1, pValue);
}

void Program::SetUniformMatrix4fv(std::string name, const float* pValue)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, pValue);
}

int Program::CompileShader(const GLenum type, const std::string& source) const
{
    GLuint shader = glCreateShader(type);

    const char *c_str = source.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    if (!CheckShaderStatus(GL_COMPILE_STATUS, shader))
    {
        glDeleteShader(shader);
        return -1;
    }

    glAttachShader(mId, shader);
    glDeleteShader(shader);

    return 0;
}

bool Program::CheckShaderStatus(const GLenum shaderType, const GLuint shaderID) const
{
    GLint success = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar log[1024] = { 0 };
        glGetShaderInfoLog(shaderID, sizeof(log), NULL, log);
        spdlog::error("Shader type {} failed to compile: {}", shaderType, log);
    }

    return success;
}

bool Program::CheckProgramStatus(const GLenum type) const
{
    GLint success = 0;
    glGetProgramiv(mId, type, &success);
    if (!success)
    {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(mId, sizeof(log), NULL, log);
        spdlog::error("Shader program failed to build: {}", log);
    }

    return success;
}

int Program::GetUniformLocation(std::string& name)
{
    auto it = mUniforms.find(name);
    if (it == mUniforms.end())
    {
        mUniforms[name] = glGetUniformLocation(mId, name.c_str());
        return mUniforms[name];
    }

    return it->second;
}
