#include "SkyboxPass.h"

#include <GL/glew.h>

#include "Renderer/Camera.h"
#include "Renderer/Context.h"
#include "Renderer/Scene.h"
#include "Renderer/Skybox.h"

namespace Renderer
{
    void SkyboxPass::Execute(const Context* context, const Scene* scene)
    {
        Camera* camera = scene->GetCamera();
        scene->GetSkybox()->Draw(context->GetProjectionMatrix(), camera->GetViewMatrixWithoutTranslation());
    }
};
