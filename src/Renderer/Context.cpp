#include "Context.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "App/Window.h"
#include "Asset/Type/Model.h"

#include "GL/Shader/Program.h"
#include "Map.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

namespace Renderer
{
    Context::Context(App::Window& window)
        : mWindow(window)
    {
    }

    const App::Window& Context::GetWindow() const
    {
        return mWindow;
    }

    glm::mat4 Context::GetProjectionMatrix() const
    {
        return glm::perspective(
            glm::radians(60.0f),
            (float)mWindow.GetWidth() / (float)mWindow.GetHeight(),
            0.1f,
            100.0f);
    }

    void Context::DrawModel(GL::Shader::Program* shaderProgram, const Model* model, bool withMaterials) const
    {
        shaderProgram->SetUniformMatrix4fv("model", glm::value_ptr(model->GetModelMatrix()));

        std::vector<Mesh*> meshes = model->GetMeshes();
        std::vector<Material*> materials = model->GetMaterials();
        std::vector<size_t> meshToMaterial = model->GetMeshToMaterialIndices();

        for (size_t meshIndex = 0; meshIndex < meshToMaterial.size(); ++meshIndex)
        {
            size_t materialIndex = meshToMaterial[meshIndex];
            Material* material = materials[materialIndex];
            Mesh* mesh = meshes[meshIndex];

            if (withMaterials)
            {
                Map* diffuseMap = material->GetMap(Asset::Type::MapTargetType::DIFFUSE);
                if (diffuseMap != nullptr)
                {
                    if (diffuseMap->GetSourceType() == Asset::Type::MapSourceType::COLOUR)
                    {
                        shaderProgram->SetUniform1i("material.useDiffuseTexture", 0);
                        shaderProgram->SetUniform3f("material.diffuseColour", diffuseMap->GetColour());
                    }
                    else
                    {
                        shaderProgram->SetUniform1i("material.useDiffuseTexture", 1);
                        shaderProgram->SetUniform1i("material.diffuseTexture", diffuseMap->GetTextureUnit());
                    }
                }

                Map* specularMap = material->GetMap(Asset::Type::MapTargetType::SPECULAR);
                if (specularMap != nullptr)
                {
                    if (specularMap->GetSourceType() == Asset::Type::MapSourceType::COLOUR)
                    {
                        shaderProgram->SetUniform1i("material.useSpecularTexture", 0);
                        shaderProgram->SetUniform3f("material.specularColour", specularMap->GetColour());
                    }
                    else
                    {
                        shaderProgram->SetUniform1i("material.useSpecularTexture", 1);
                        shaderProgram->SetUniform1i("material.specularTexture", specularMap->GetTextureUnit());
                    }
                }

                shaderProgram->SetUniform1f("material.specularIntensity", material->GetSpecularIntensity());
                shaderProgram->SetUniform1f("material.shininess", material->GetShininess());
            }

            material->Bind();
            mesh->Bind();
            mesh->Draw();
            mesh->Unbind();
            material->Unbind();
        }
    }

    void Context::DrawModels(GL::Shader::Program* shaderProgram, std::vector<Model*> models, bool withMaterials) const
    {
        shaderProgram->Use();

        for (size_t i = 0; i < models.size(); ++i)
        {
            DrawModel(shaderProgram, models[i], withMaterials);
        }
    }
};
