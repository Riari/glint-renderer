#include "ScenePass.h"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Camera.h"
#include "Renderer/Constants.h"
#include "Renderer/Context.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Scene.h"
#include "Util/File.h"

namespace Renderer
{
    ScenePass::ScenePass()
        : mBasicMaterialShader(new GL::Shader::Program())
    {
    }

    ScenePass::~ScenePass()
    {
        delete mBasicMaterialShader;
    }

    bool ScenePass::Init() const
    {
        mBasicMaterialShader->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/basic-material.vert.glsl"));
        mBasicMaterialShader->AddShader(GL_FRAGMENT_SHADER, Util::File::Read("shaders/basic-material.frag.glsl"));
        return mBasicMaterialShader->Link() == 0;
    }

    void ScenePass::Execute(const Context* context, const Scene* scene)
    {
        mBasicMaterialShader->Use();

        Camera* camera = scene->GetCamera();
        DirectionalLight* directionalLight = scene->GetDirectionalLight();

        mBasicMaterialShader->SetUniformMatrix4fv("view", glm::value_ptr(camera->GetViewMatrix()));
        mBasicMaterialShader->SetUniformMatrix4fv("projection", glm::value_ptr(context->GetProjectionMatrix()));
        mBasicMaterialShader->SetUniform3f("directionalLight.base.colour", directionalLight->GetColour());
        mBasicMaterialShader->SetUniform1f("directionalLight.base.ambientIntensity", directionalLight->GetAmbientIntensity());
        mBasicMaterialShader->SetUniform1f("directionalLight.base.diffuseIntensity", directionalLight->GetDiffuseIntensity());
        mBasicMaterialShader->SetUniform3f("directionalLight.direction", directionalLight->GetDirection());

        directionalLight->GetShadowMap()->GetMap()->Bind();
        mBasicMaterialShader->SetUniform1i("directionalShadowMap", directionalLight->GetShadowMap()->GetMap()->GetUnit());
        glm::mat4 directionalLightTransform = directionalLight->CalculateLightTransform();
        mBasicMaterialShader->SetUniformMatrix4fv("directionalLightTransform", glm::value_ptr(directionalLightTransform));

        mBasicMaterialShader->SetUniform3f("eyePosition", camera->GetPosition());

        std::vector<PointLight*> pointLights = scene->GetPointLights();
        mBasicMaterialShader->SetUniform1i("pointLightCount", pointLights.size());
        for (size_t i = 0; i < pointLights.size(); ++i)
        {
            std::string light = "pointLights[" + std::to_string(i) + "]";
            mBasicMaterialShader->SetUniform3f(light + ".base.colour", pointLights[i]->GetColour());
            mBasicMaterialShader->SetUniform1f(light + ".base.ambientIntensity", pointLights[i]->GetAmbientIntensity());
            mBasicMaterialShader->SetUniform1f(light + ".base.diffuseIntensity", pointLights[i]->GetDiffuseIntensity());
            mBasicMaterialShader->SetUniform3f(light + ".position", pointLights[i]->GetPosition());
            mBasicMaterialShader->SetUniform1f(light + ".constant", pointLights[i]->GetConstant());
            mBasicMaterialShader->SetUniform1f(light + ".linear", pointLights[i]->GetLinear());
            mBasicMaterialShader->SetUniform1f(light + ".exponent", pointLights[i]->GetExponent());

            pointLights[i]->GetShadowMap()->GetMap()->Bind();

            std::string shadowMap = "omniShadowMaps[" + std::to_string(i) + "]";
            mBasicMaterialShader->SetUniform1i(shadowMap + ".map", pointLights[i]->GetShadowMap()->GetMap()->GetUnit());
        }

        std::vector<SpotLight*> spotLights = scene->GetSpotLights();
        mBasicMaterialShader->SetUniform1i("spotLightCount", spotLights.size());
        for (size_t i = 0; i < spotLights.size(); ++i)
        {
            std::string light = "spotLights[" + std::to_string(i) + "]";
            mBasicMaterialShader->SetUniform3f(light + ".base.base.colour", spotLights[i]->GetColour());
            mBasicMaterialShader->SetUniform1f(light + ".base.base.ambientIntensity", spotLights[i]->GetAmbientIntensity());
            mBasicMaterialShader->SetUniform1f(light + ".base.base.diffuseIntensity", spotLights[i]->GetDiffuseIntensity());
            mBasicMaterialShader->SetUniform3f(light + ".base.position", spotLights[i]->GetPosition());
            mBasicMaterialShader->SetUniform1f(light + ".base.constant", spotLights[i]->GetConstant());
            mBasicMaterialShader->SetUniform1f(light + ".base.linear", spotLights[i]->GetLinear());
            mBasicMaterialShader->SetUniform1f(light + ".base.exponent", spotLights[i]->GetExponent());
            mBasicMaterialShader->SetUniform3f(light + ".direction", spotLights[i]->GetDirection());
            mBasicMaterialShader->SetUniform1f(light + ".edgeCosine", spotLights[i]->GetEdgeCosine());

            spotLights[i]->GetShadowMap()->GetMap()->Bind();

            std::string shadowMap = "omniShadowMaps[" + std::to_string(spotLights.size() + i) + "]";
            mBasicMaterialShader->SetUniform1i(shadowMap + ".map", spotLights[i]->GetShadowMap()->GetMap()->GetUnit());
        }

        mBasicMaterialShader->SetUniform1f("farPlane", Renderer::FAR_PLANE);

        context->DrawModels(mBasicMaterialShader, scene->GetModels(), true);
    }
};
