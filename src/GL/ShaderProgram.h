#pragma once

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

        void SetUniform1i(const char* pName, const int value) const;
        void SetUniform1f(const char* pName, const float value) const;
        void SetUniform4fv(const char* pName, const float* pValue) const;
        void SetUniformMatrix4fv(const char* pName, const float* pValue) const;

    private:
        bool mIsBuilt{false};

        Shader* pVertex;
        Shader* pFragment;
    };
}
