#pragma once

#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

#include "Object.h"
#include "Shader.h"

namespace Renderer::GL
{
    class ShaderProgram : public Object
    {
    public:
        ShaderProgram();

        int Build(const std::string& vertexSource, const std::string& fragmentSource);

        void Use() const;

        void SetUniform1i(std::string name, const int value);
        void SetUniform1f(std::string name, const float value);
        void SetUniform3f(std::string name, const glm::vec3 value);
        void SetUniform4fv(std::string name, const float* pValue);
        void SetUniformMatrix4fv(std::string name, const float* pValue);

    private:
        bool mIsBuilt{false};

        Shader* mVertex;
        Shader* mFragment;

        std::unordered_map<std::string, int> mUniforms;

        int GetUniformLocation(std::string& name);
    };
}
