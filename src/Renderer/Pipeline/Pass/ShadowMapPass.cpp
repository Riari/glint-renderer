#include "ShadowMapPass.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Camera.h"
#include "Renderer/Constants.h"
#include "Renderer/Context.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Scene.h"
#include "Renderer/ShadowMap.h"
#include "Util/File.h"

namespace Renderer
{
    ShadowMapPass::ShadowMapPass()
        : mDirectionalShader(new GL::Shader::Program())
        , mOmnidirectionalShader(new GL::Shader::Program())
    {
    }

    ShadowMapPass::~ShadowMapPass()
    {
        delete mDirectionalShader;
        delete mOmnidirectionalShader;
    }

    bool ShadowMapPass::Init() const
    {
        mDirectionalShader->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/directional-shadow-map.vert.glsl"));
        if (mDirectionalShader->Link() != 0)
        {
            return false;
        }

        mOmnidirectionalShader->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/omnidirectional-shadow-map.vert.glsl"));
        mOmnidirectionalShader->AddShader(GL_GEOMETRY_SHADER, Util::File::Read("shaders/omnidirectional-shadow-map.geom.glsl"));
        mOmnidirectionalShader->AddShader(GL_FRAGMENT_SHADER, Util::File::Read("shaders/omnidirectional-shadow-map.frag.glsl"));
        return mOmnidirectionalShader->Link() == 0;
    }

    void ShadowMapPass::Execute(const Context* context, const Scene* scene)
    {
        mDirectionalShader->Use();

        DirectionalLight* directionalLight = scene->GetDirectionalLight();

        ShadowMap* shadowMap = directionalLight->GetShadowMap();
        glViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());

        shadowMap->BindFramebuffer();
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 directionalLightTransform = directionalLight->CalculateLightTransform();
        mDirectionalShader->SetUniformMatrix4fv("directionalLightTransform", glm::value_ptr(directionalLightTransform));

        context->DrawModels(mDirectionalShader, scene->GetModels(), false);

        shadowMap->UnbindFramebuffer();

        std::vector<PointLight*> pointLights = scene->GetPointLights();
        for (size_t i = 0; i < pointLights.size(); ++i)
        {
            ProcessOmnidirectionalLight(context, scene, pointLights[i]);
        }

        std::vector<SpotLight*> spotLights = scene->GetSpotLights();
        for (size_t i = 0; i < spotLights.size(); ++i)
        {
            ProcessOmnidirectionalLight(context, scene, spotLights[i]);
        }
    }

    void ShadowMapPass::ProcessOmnidirectionalLight(const Context* context, const Scene* scene, const PointLight* light) const
    {
        mOmnidirectionalShader->Use();

        Renderer::ShadowMap* shadowMap = light->GetShadowMap();
        glViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());

        shadowMap->BindFramebuffer();
        glClear(GL_DEPTH_BUFFER_BIT);

        mOmnidirectionalShader->SetUniform3f("lightPosition", light->GetPosition());
        mOmnidirectionalShader->SetUniform1f("farPlane", Renderer::FAR_PLANE);

        std::vector<glm::mat4> lightMatrices = light->CalculateLightTransforms();

        for (size_t i = 0; i < lightMatrices.size(); ++i)
        {
            std::string matrix = "lightMatrices[" + std::to_string(i) + "]";
            mOmnidirectionalShader->SetUniformMatrix4fv(matrix, glm::value_ptr(lightMatrices[i]));
        }

        context->DrawModels(mOmnidirectionalShader, scene->GetModels(), false);

        shadowMap->UnbindFramebuffer();
    }
};
