#pragma once

#include "Renderer/GL/Shader/Program.h"
#include "Renderer/Pipeline/IPass.h"

namespace Renderer
{
    class ScenePass : public IPass
    {
    public:
        ScenePass();
        ~ScenePass();

        bool Init() const override;
        void Execute(const Context* context, const Scene* scene) override;

    private:
        GL::Shader::Program* mBasicMaterialShader;
    };
};
