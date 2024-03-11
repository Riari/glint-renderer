#pragma once

#include <vector>

#include "Renderer/Context.h"
#include "Renderer/Scene.h"

#include "IPass.h"

namespace Renderer
{
    class Pipeline
    {
    public:
        Pipeline() = default;
        ~Pipeline();

        bool Init();
        void AddPass(IPass* pass);
        void Execute(const Context* context, const Scene* scene);

    private:
        std::vector<IPass*> mPasses;
    };
};
