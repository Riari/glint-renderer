#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "Renderer/GL/Object.h"

namespace Renderer::GL::Shader
{
    class Program : public Object
    {
    public:
        Program();
        ~Program();

        int AddShader(const GLuint shaderType, const std::string& source);
        int Link();

        void Use() const;

        template<typename... Names>
        void RegisterUniforms(Names&&... names)
        {
            auto getLocationAndAdd = [&mUniforms](const auto& name)
            {
                mUniforms[name] = GetUniformLocation(name);
            };

            (getLocationAndAdd(names), ...);
        }

        void SetUniform1i(std::string name, const int value);
        void SetUniform1f(std::string name, const float value);
        void SetUniform3f(std::string name, const glm::vec3& value);
        void SetUniform4fv(std::string name, const float* pValue);
        void SetUniformMatrix4fv(std::string name, const float* pValue);

    private:
        bool mIsBuilt{false};

        std::vector<GLuint> mShaderTypes;
        std::unordered_map<std::string, GLuint> mUniforms;

        int CompileShader(const GLenum type, const std::string& source) const;

        bool CheckShaderStatus(const GLenum shaderType, const GLuint shader) const;
        bool CheckProgramStatus(const GLuint program) const;

        int GetUniformLocation(std::string& name);
    };
}
