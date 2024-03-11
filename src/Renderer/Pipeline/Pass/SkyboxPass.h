#pragma once

#include "Renderer/Pipeline/IPass.h"

namespace Renderer
{
    class SkyboxPass : public IPass
    {
    public:
        SkyboxPass() = default;
        ~SkyboxPass() = default;

        void Execute(const Context* context, const Scene* scene) override;
    };
};
