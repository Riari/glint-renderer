#pragma once

#include "Renderer/GL/Shader/Program.h"
#include "Renderer/Pipeline/IPass.h"

namespace Renderer
{
    class PointLight;

    class ShadowMapPass : public IPass
    {
    public:
        ShadowMapPass();
        ~ShadowMapPass();

        bool Init() const override;
        void Execute(const Context* context, const Scene* scene) override;
        void ProcessOmnidirectionalLight(const Context* context, const Scene* scene, const PointLight* light) const;

    private:
        GL::Shader::Program* mDirectionalShader;
        GL::Shader::Program* mOmnidirectionalShader;
    };
};
