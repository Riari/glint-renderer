#pragma once

#include <string>

#include <glm/glm.hpp>

#include "GL/Shader/Program.h"
#include "GL/Texture.h"
#include "Mesh.h"

namespace Renderer
{
    class Skybox
    {
    public:
        Skybox(const std::string& name);
        ~Skybox();

        void Draw(const glm::mat4& projection, const glm::mat4& view) const;

    private:
        GL::Shader::Program* mShaderProgram;
        Mesh* mMesh;
        GL::Texture* mTexture;
    };
};
