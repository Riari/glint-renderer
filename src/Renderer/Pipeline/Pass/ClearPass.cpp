#include "ClearPass.h"

#include "Renderer/Constants.h"

namespace Renderer
{
    void ClearPass::Execute(const Context* context, const Scene* scene)
    {
        glViewport(0, 0, Renderer::VIEWPORT_WIDTH, Renderer::VIEWPORT_HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};
