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

    void Pipeline::AddPass(IPass* pass)
    {
        mPasses.push_back(pass);
    }

    void Pipeline::Execute()
    {
        for (auto pass : mPasses)
        {
            pass->Execute();
        }
    }
};
