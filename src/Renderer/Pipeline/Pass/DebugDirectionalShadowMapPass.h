#pragma once

#include "Renderer/Pipeline/IPass.h"

namespace Renderer
{
    namespace GL::Shader { class Program; }
    class Model;

    class DebugDirectionalShadowMapPass : public IPass
    {
    public:
        DebugDirectionalShadowMapPass();
        ~DebugDirectionalShadowMapPass();

        bool Init() const override;
        void Execute(const Context* context, const Scene* scene) override;

    private:
        GL::Shader::Program* mShader;
        Model* mQuad;
    };
};
