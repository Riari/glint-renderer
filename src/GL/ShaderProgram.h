#pragma once

#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

#include "Object.h"
#include "Shader.h"

namespace GL
{
    class ShaderProgram : public Object
    {
    public:
        ShaderProgram(const char* vertexSource, const char* fragmentSource);

        int Build();

        void Use() const;

        void SetUniform1i(const char* pName, const int value);
        void SetUniform1f(const char* pName, const float value);
        void SetUniform3f(const char* pName, const glm::vec3 value);
        void SetUniform4fv(const char* pName, const float* pValue);
        void SetUniformMatrix4fv(const char* pName, const float* pValue);

    private:
        bool mIsBuilt{false};

        Shader* pVertex;
        Shader* pFragment;

        std::unordered_map<std::string, int> mUniforms;

        int GetUniformLocation(const char* pName);
    };
}
