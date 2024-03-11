#include "Pipeline.h"

namespace Renderer
{
    Pipeline::~Pipeline()
    {
        for (auto pass : mPasses)
        {
            delete pass;
        }
        mPasses.clear();
    }

    bool Pipeline::Init()
    {
        for (auto pass : mPasses)
        {
            if (!pass->Init())
            {
                return false;
            }
        }

        return true;
    }

    void Pipeline::AddPass(IPass* pass)
    {
        mPasses.push_back(pass);
    }

    void Pipeline::Execute(const Context* context, const Scene* scene)
    {
        for (auto pass : mPasses)
        {
            pass->Execute(context, scene);
        }
    }
};
