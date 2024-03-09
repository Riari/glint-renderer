#pragma once

#include "Renderer/Pipeline/IPass.h"

namespace Renderer
{
    class ClearPass : public IPass
    {
    public:
        ClearPass() = default;
        ~ClearPass() = default;

        void Execute() override;
    };
};
