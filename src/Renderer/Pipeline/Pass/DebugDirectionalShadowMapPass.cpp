#include "DebugDirectionalShadowMapPass.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App/Window.h"
#include "Renderer/Camera.h"
#include "Renderer/Constants.h"
#include "Renderer/Context.h"
#include "Renderer/GL/Shader/Program.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Scene.h"
#include "Renderer/Model.h"
#include "Util/File.h"

namespace Renderer
{
    DebugDirectionalShadowMapPass::DebugDirectionalShadowMapPass()
        : mShader(new GL::Shader::Program())
        , mQuad(new Model())
    {
    }

    DebugDirectionalShadowMapPass::~DebugDirectionalShadowMapPass()
    {
        delete mShader;
    }

    bool DebugDirectionalShadowMapPass::Init() const
    {
        mQuad->AddMesh(
            new Renderer::Mesh(Renderer::PRIMITIVE_QUAD_VERTICES, Renderer::PRIMITIVE_QUAD_INDICES, 5, { 3, 2 }, false),
            new Renderer::Material(0.0f, 0.0f));

        mShader->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/debug-quad.vert.glsl"));
        mShader->AddShader(GL_FRAGMENT_SHADER, Util::File::Read("shaders/debug-quad.frag.glsl"));
        return mShader->Link() == 0;
    }

    void DebugDirectionalShadowMapPass::Execute(const Context* context, const Scene* scene)
    {
        const App::Window& window = context->GetWindow();
        DirectionalLight* directionalLight = scene->GetDirectionalLight();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.GetBufferWidth(), window.GetBufferHeight());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        directionalLight->GetShadowMap()->GetMap()->Bind();
        mShader->Use();
        mShader->SetUniform1i("textureSampler", directionalLight->GetShadowMap()->GetMap()->GetUnit());
        context->DrawModel(mShader, mQuad, false);
    }
};
