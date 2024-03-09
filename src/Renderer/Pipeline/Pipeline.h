#pragma once

#include <vector>

#include "IPass.h"

namespace Renderer
{
    class Pipeline
    {
    public:
        Pipeline() = default;
        ~Pipeline();

        void AddPass(IPass* pass);
        void Execute();

    private:
        std::vector<IPass*> mPasses;
    };
};
