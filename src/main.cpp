#include <vector>

#include <assimp/Importer.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "App/Window.h"
#include "Asset/Type/Image.h"
#include "Asset/Type/Model.h"
#include "Asset/Type/Skybox.h"
#include "Asset/Asset.h"
#include "Asset/Manager.h"
#include "Renderer/GL/Shader/Program.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/Light.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Camera.h"
#include "Renderer/Constants.h"
#include "Renderer/Map.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/Model.h"
#include "Renderer/Skybox.h"
#include "Util/GL.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool init();
bool loop();

App::Window* gWindow;
Renderer::Camera* gCamera;
Renderer::DirectionalLight* gDirectionalLight;
std::vector<Renderer::PointLight*> gPointLights;
std::vector<Renderer::SpotLight*> gSpotLights;
Renderer::GL::Shader::Program* gBasicShader;
Renderer::GL::Shader::Program* gDirectionalShadowMapShader;
Renderer::GL::Shader::Program* gOmnidirectionalShadowMapShader;
Renderer::GL::Shader::Program* gDebugQuadShader;
Renderer::Skybox* gSkybox;

std::vector<Renderer::Model*> gTestModels;
Renderer::Model* gDebugQuad;

int gFramesPerSecond;
double gDeltaTime;

int main()
{
    spdlog::info("Beginning initialisation...");
    if (!init())
    {
        spdlog::error("Initialisation failed");
        return -1;
    }

    double previousTime = glfwGetTime();
    double previousFPSUpdateTime = previousTime;
    int frameCount{0};

    spdlog::info("Entering main loop...");
    while (loop())
    {
        double currentTime = glfwGetTime();
        frameCount++;

        gDeltaTime = currentTime - previousTime;
        previousTime = currentTime;

        if (currentTime - previousFPSUpdateTime >= 1.0)
        {
            gFramesPerSecond = frameCount;
            frameCount = 0;
            previousFPSUpdateTime = currentTime;
        }
    }

    spdlog::info("Cleaning up...");

    delete gWindow;
    delete gCamera;
    delete gDirectionalLight;

    for (auto pointLight : gPointLights)
    {
        delete pointLight;
    }
    gPointLights.clear();

    for (auto spotLight : gSpotLights)
    {
        delete spotLight;
    }
    gSpotLights.clear();

    delete gBasicShader;
    delete gDirectionalShadowMapShader;
    delete gOmnidirectionalShadowMapShader;
    delete gDebugQuadShader;

    for (auto model : gTestModels)
    {
        delete model;
    }
    gTestModels.clear();

    delete gDebugQuad;
    delete gSkybox;

    glfwTerminate();

    return 0;
}

static void glfw_error_callback(int error, const char* description)
{
    spdlog::error("Error {}: {}", error, description);
}

bool init()
{
    spdlog::info("Initialising GLFW");

    // GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return false;
    }

    {
        gWindow = new App::Window(Renderer::VIEWPORT_WIDTH, Renderer::VIEWPORT_HEIGHT, "udemy-opengl");
        gWindow->MakeCurrent();
    }

    spdlog::info("Initialising GLEW");

    // GLEW
    if (glewInit() != GLEW_OK)
    {
        return false;
    }

    spdlog::info("Setting OpenGL flags");

    // OpenGL flags
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(Util::GL::HandleDebugMessage, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }

    spdlog::info("Building shaders");
    {
        gBasicShader = new Renderer::GL::Shader::Program();
        gBasicShader->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/basic-material.vert.glsl"));
        gBasicShader->AddShader(GL_FRAGMENT_SHADER, Util::File::Read("shaders/basic-material.frag.glsl"));
        if (gBasicShader->Link() != 0)
        {
            glfwTerminate();
            return false;
        }

        gDirectionalShadowMapShader = new Renderer::GL::Shader::Program();
        gDirectionalShadowMapShader->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/directional-shadow-map.vert.glsl"));
        if (gDirectionalShadowMapShader->Link() != 0)
        {
            glfwTerminate();
            return false;
        }

        gOmnidirectionalShadowMapShader = new Renderer::GL::Shader::Program();
        gOmnidirectionalShadowMapShader->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/omnidirectional-shadow-map.vert.glsl"));
        gOmnidirectionalShadowMapShader->AddShader(GL_GEOMETRY_SHADER, Util::File::Read("shaders/omnidirectional-shadow-map.geom.glsl"));
        gOmnidirectionalShadowMapShader->AddShader(GL_FRAGMENT_SHADER, Util::File::Read("shaders/omnidirectional-shadow-map.frag.glsl"));
        if (gOmnidirectionalShadowMapShader->Link() != 0)
        {
            glfwTerminate();
            return false;
        }

        gDebugQuadShader = new Renderer::GL::Shader::Program();
        gDebugQuadShader->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/debug-quad.vert.glsl"));
        gDebugQuadShader->AddShader(GL_FRAGMENT_SHADER, Util::File::Read("shaders/debug-quad.frag.glsl"));
        if (gDebugQuadShader->Link() != 0)
        {
            glfwTerminate();
            return false;
        }
    }

    spdlog::info("Registering assets");
    {
        stbi_set_flip_vertically_on_load(true);

        Asset::Manager::RegisterInitialiser(".jpg", Asset::Type::InitialiseImage);
        Asset::Manager::RegisterInitialiser(".png", Asset::Type::InitialiseImage);
        Asset::Manager::RegisterLoader("Image", Asset::Type::LoadImage);
        Asset::Manager::RegisterLoader("Model", Asset::Type::ModelLoader::Load);
        Asset::Manager::RegisterLoader("Skybox", Asset::Type::LoadSkybox);

        Asset::Manager::Initialise();
        Asset::Manager::LoadAssets();
    }

    spdlog::info("Creating world objects");
    {
        gCamera = new Renderer::Camera(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            -60.0f, 0.0f,
            1.0f,
            0.3f);

        gDirectionalLight = new Renderer::DirectionalLight(
            glm::vec3(1.0f, 0.6f, 0.4f),
            0.2f,
            1.5f,
            glm::vec3(0.0f, -25.0f, -10.0f),
            5);

        // gPointLights.push_back(new Renderer::PointLight(
        //     glm::vec3(1.0f, 0.0f, 0.5f),
        //     0.1f,
        //     2.0f,
        //     glm::vec3(0.0f, 2.5f, -3.0f),
        //     0.3f,
        //     0.1f,
        //     0.1f,
        //     6));
        // gPointLights.push_back(new Renderer::PointLight(
        //     glm::vec3(0.0f, 0.75f, 1.0f),
        //     0.1f,
        //     2.0f,
        //     glm::vec3(3.0f, 2.5f, 0.0f),
        //     0.3f,
        //     0.1f,
        //     0.1f,
        //     7));
        // gPointLights.push_back(new Renderer::PointLight(
        //     glm::vec3(1.0f, 0.42f, 0.0f),
        //     0.1f,
        //     2.0f,
        //     glm::vec3(-3.0f, 2.5f, 0.0f),
        //     0.3f,
        //     0.1f,
        //     0.1f,
        //     8));
        // gPointLights.push_back(new Renderer::PointLight(
        //     glm::vec3(0.47f, 0.0f, 1.0f),
        //     0.1f,
        //     2.0f,
        //     glm::vec3(0.0f, 2.5f, 3.0f),
        //     0.3f,
        //     0.1f,
        //     0.1f,
        //     9));

        // gSpotLights.push_back(new Renderer::SpotLight(
        //     glm::vec3(1.0f, 1.0f, 1.0f),
        //     0.0f,
        //     2.5f,
        //     glm::vec3(-2.0f, 2.0f, 0.0f),
        //     glm::vec3(0.0f, -1.0f, 0.0f),
        //     1.0f,
        //     0.2f,
        //     0.1f,
        //     40.0f,
        //     10));
        // gSpotLights.push_back(new Renderer::SpotLight(
        //     glm::vec3(1.0f, 1.0f, 1.0f),
        //     0.0f,
        //     2.5f,
        //     glm::vec3(2.0f, 2.0f, 0.0f),
        //     glm::vec3(0.0f, -1.0f, 0.0f),
        //     1.0f,
        //     0.2f,
        //     0.1f,
        //     40.0f,
        //     11));

        gTestModels.push_back(new Renderer::Model());
        gTestModels.push_back(new Renderer::Model());
        gTestModels.push_back(new Renderer::Model());
        gTestModels.push_back(new Renderer::Model());

        auto mapBrick = new Renderer::Map(Asset::Manager::Get<Asset::Type::Image>("Image::Brick"), Asset::Type::MapTargetType::DIFFUSE);
        auto mapDirt = new Renderer::Map(Asset::Manager::Get<Asset::Type::Image>("Image::Dirt"), Asset::Type::MapTargetType::DIFFUSE);
        auto mapStainlessSteel = new Renderer::Map(Asset::Manager::Get<Asset::Type::Image>("Image::StainlessSteel"), Asset::Type::MapTargetType::DIFFUSE);

        auto materialBrick = new Renderer::Material(1.0f, 32.0f);
        materialBrick->SetMap(mapBrick);

        auto materialDirt = new Renderer::Material(0.3f, 4.0f);
        materialDirt->SetMap(mapDirt);

        auto materialStainlessSteel = new Renderer::Material(2.0f, 128.0f);
        materialStainlessSteel->SetMap(mapStainlessSteel);
        materialStainlessSteel->SetMap(new Renderer::Map(glm::vec3(1.0f, 1.0f, 1.0f), Asset::Type::MapTargetType::SPECULAR));

        gTestModels[0]->AddMesh(
            new Renderer::Mesh(Renderer::PRIMITIVE_PLANE_VERTICES, Renderer::PRIMITIVE_PLANE_INDICES, 8, { 3, 2, 3 }, false),
            materialStainlessSteel);
        gTestModels[0]->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));

        // TODO: Define a better way of converting imported models and materials into renderer-ready models
        const Asset::Type::Model& model = Asset::Manager::Get<Asset::Type::Model>("Model::F1Car");
        for (size_t i = 0; i < model.mMaterials.size(); ++i)
        {
            Asset::Type::MaterialData* materialData = model.mMaterials[i];
            Renderer::Material* material = new Renderer::Material(materialData->mSpecularIntensity, materialData->mShininess);

            for (size_t j = 0; j < materialData->mMaps.size(); ++j)
            {
                Asset::Type::MapData* mapData = materialData->mMaps[j];

                if (mapData->mSourceType == Asset::Type::MapSourceType::IMAGE)
                {
                    Renderer::Map* map = new Renderer::Map(Asset::Manager::Get<Asset::Type::Image>(mapData->mAssetName), mapData->mTargetType);
                    material->SetMap(map);
                }
                else
                {
                    Renderer::Map* map = new Renderer::Map(mapData->mColour, mapData->mTargetType);
                    material->SetMap(map);
                }
            }

            gTestModels[1]->AddMaterial(material);
        }

        for (size_t i = 0; i < model.mMeshes.size(); ++i)
        {
            Asset::Type::MeshData* mesh = model.mMeshes[i];

            gTestModels[1]->AddMesh(
                new Renderer::Mesh(mesh->mVertices, mesh->mIndices, mesh->mVertexLength, mesh->mVertexLayout, false),
                mesh->mMaterialIndex);
        }

        gTestModels[1]->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
        gTestModels[1]->SetScale(glm::vec3(0.01f));

        gSkybox = new Renderer::Skybox("Skybox::yellowcloud");
    }

    gDebugQuad = new Renderer::Model();
    gDebugQuad->AddMesh(
        new Renderer::Mesh(Renderer::PRIMITIVE_QUAD_VERTICES, Renderer::PRIMITIVE_QUAD_INDICES, 5, { 3, 2 }, false),
        new Renderer::Material(0.0f, 0.0f));

    spdlog::info("Initialisation complete!");

    return true;
}

void DrawModel(Renderer::GL::Shader::Program* shaderProgram, const Renderer::Model* model, bool withMaterials)
{
    shaderProgram->SetUniformMatrix4fv("model", glm::value_ptr(model->GetModelMatrix()));

    std::vector<Renderer::Mesh*> meshes = model->GetMeshes();
    std::vector<Renderer::Material*> materials = model->GetMaterials();
    std::vector<size_t> meshToMaterial = model->GetMeshToMaterialIndices();

    for (size_t meshIndex = 0; meshIndex < meshToMaterial.size(); ++meshIndex)
    {
        size_t materialIndex = meshToMaterial[meshIndex];
        Renderer::Material* material = materials[materialIndex];
        Renderer::Mesh* mesh = meshes[meshIndex];

        if (withMaterials)
        {
            Renderer::Map* diffuseMap = material->GetMap(Asset::Type::MapTargetType::DIFFUSE);
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

            Renderer::Map* specularMap = material->GetMap(Asset::Type::MapTargetType::SPECULAR);
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

void DrawModels(Renderer::GL::Shader::Program* shaderProgram, bool withMaterials)
{
    shaderProgram->Use();

    for (size_t i = 0; i < gTestModels.size(); ++i)
    {
        DrawModel(shaderProgram, gTestModels[i], withMaterials);
    }
}

static void RenderPassOmnidirectionalLight(Renderer::PointLight* light)
{
    gOmnidirectionalShadowMapShader->Use();

    Renderer::ShadowMap* shadowMap = light->GetShadowMap();
    glViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());

    shadowMap->BindFramebuffer();
    glClear(GL_DEPTH_BUFFER_BIT);

    gOmnidirectionalShadowMapShader->SetUniform3f("lightPosition", light->GetPosition());
    gOmnidirectionalShadowMapShader->SetUniform1f("farPlane", Renderer::FAR_PLANE);

    std::vector<glm::mat4> lightMatrices = light->CalculateLightTransforms();

    for (size_t i = 0; i < lightMatrices.size(); ++i)
    {
        std::string matrix = "lightMatrices[" + std::to_string(i) + "]";
        gOmnidirectionalShadowMapShader->SetUniformMatrix4fv(matrix, glm::value_ptr(lightMatrices[i]));
    }

    DrawModels(gOmnidirectionalShadowMapShader, false);

    shadowMap->UnbindFramebuffer();
}

bool loop()
{
    if (gWindow->ShouldClose())
    {
        return false;
    }

    glfwPollEvents();

    // TODO: Come up with a better way of managing the interface between world objects (e.g. lights) and shader programs
    // TODO: Validate shader inputs (e.g. point light count must not exceed MAX_POINT_LIGHTS)
    // TODO: Encapsulate render passes


    if (gWindow->IsKeyPressed(GLFW_KEY_UP))
    {
        glm::vec3 direction = gDirectionalLight->GetDirection();
        direction.y += 0.1f;
        gDirectionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_DOWN))
    {
        glm::vec3 direction = gDirectionalLight->GetDirection();
        direction.y -= 0.1f;
        gDirectionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_LEFT))
    {
        glm::vec3 direction = gDirectionalLight->GetDirection();
        direction.x -= 0.1f;
        gDirectionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_RIGHT))
    {
        glm::vec3 direction = gDirectionalLight->GetDirection();
        direction.x += 0.1f;
        gDirectionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_O))
    {
        glm::vec3 direction = gDirectionalLight->GetDirection();
        direction.z += 0.1f;
        gDirectionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_L))
    {
        glm::vec3 direction = gDirectionalLight->GetDirection();
        direction.z -= 0.1f;
        gDirectionalLight->SetDirection(direction);
    }

    // Render pass: directional shadow maps
    {
        gDirectionalShadowMapShader->Use();

        Renderer::ShadowMap* shadowMap = gDirectionalLight->GetShadowMap();
        glViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());

        shadowMap->BindFramebuffer();
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 directionalLightTransform = gDirectionalLight->CalculateLightTransform();
        gDirectionalShadowMapShader->SetUniformMatrix4fv("directionalLightTransform", glm::value_ptr(directionalLightTransform));

        DrawModels(gDirectionalShadowMapShader, false);

        shadowMap->UnbindFramebuffer();
    }

    // Render pass: visualize directional shadow depth buffer
    {
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glViewport(0, 0, gWindow->GetBufferWidth(), gWindow->GetBufferHeight());
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // gDirectionalLight->GetShadowMap()->GetMap()->Bind();
        // gDebugQuadShader->Use();
        // gDebugQuadShader->SetUniform1i("textureSampler", gDirectionalLight->GetShadowMap()->GetMap()->GetUnit());
        // DrawModel(gDebugQuadShader, gDebugQuad, false);
    }

    // Render pass: omnidirectional shadow maps
    {
        for (size_t i = 0; i < gPointLights.size(); ++i)
        {
            RenderPassOmnidirectionalLight(gPointLights[i]);
        }

        for (size_t i = 0; i < gSpotLights.size(); ++i)
        {
            RenderPassOmnidirectionalLight(gSpotLights[i]);
        }
    }

    // Render pass: clear for scene
    {
        glViewport(0, 0, Renderer::VIEWPORT_WIDTH, Renderer::VIEWPORT_HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)gWindow->GetWidth() / (float)gWindow->GetHeight(), 0.1f, 100.0f);
    gCamera->Update(gWindow, static_cast<float>(gDeltaTime));
    glm::mat4 view = gCamera->GetViewMatrix();
    glm::mat4 untranslatedView = glm::mat4(glm::mat3(view));

    // Render pass: skybox
    {
        gSkybox->Draw(projection, untranslatedView);
    }

    // Render pass: scene
    {
        gBasicShader->Use();

        gBasicShader->SetUniformMatrix4fv("view", glm::value_ptr(view));
        gBasicShader->SetUniformMatrix4fv("projection", glm::value_ptr(projection));
        gBasicShader->SetUniform3f("directionalLight.base.colour", gDirectionalLight->GetColour());
        gBasicShader->SetUniform1f("directionalLight.base.ambientIntensity", gDirectionalLight->GetAmbientIntensity());
        gBasicShader->SetUniform1f("directionalLight.base.diffuseIntensity", gDirectionalLight->GetDiffuseIntensity());
        gBasicShader->SetUniform3f("directionalLight.direction", gDirectionalLight->GetDirection());

        gDirectionalLight->GetShadowMap()->GetMap()->Bind();
        gBasicShader->SetUniform1i("directionalShadowMap", gDirectionalLight->GetShadowMap()->GetMap()->GetUnit());
        glm::mat4 directionalLightTransform = gDirectionalLight->CalculateLightTransform();
        gBasicShader->SetUniformMatrix4fv("directionalLightTransform", glm::value_ptr(directionalLightTransform));

        gBasicShader->SetUniform3f("eyePosition", gCamera->GetPosition());

        gBasicShader->SetUniform1i("pointLightCount", gPointLights.size());
        for (size_t i = 0; i < gPointLights.size(); ++i)
        {
            std::string light = "pointLights[" + std::to_string(i) + "]";
            gBasicShader->SetUniform3f(light + ".base.colour", gPointLights[i]->GetColour());
            gBasicShader->SetUniform1f(light + ".base.ambientIntensity", gPointLights[i]->GetAmbientIntensity());
            gBasicShader->SetUniform1f(light + ".base.diffuseIntensity", gPointLights[i]->GetDiffuseIntensity());
            gBasicShader->SetUniform3f(light + ".position", gPointLights[i]->GetPosition());
            gBasicShader->SetUniform1f(light + ".constant", gPointLights[i]->GetConstant());
            gBasicShader->SetUniform1f(light + ".linear", gPointLights[i]->GetLinear());
            gBasicShader->SetUniform1f(light + ".exponent", gPointLights[i]->GetExponent());

            gPointLights[i]->GetShadowMap()->GetMap()->Bind();

            std::string shadowMap = "omniShadowMaps[" + std::to_string(i) + "]";
            gBasicShader->SetUniform1i(shadowMap + ".map", gPointLights[i]->GetShadowMap()->GetMap()->GetUnit());
        }

        gBasicShader->SetUniform1i("spotLightCount", gSpotLights.size());
        for (size_t i = 0; i < gSpotLights.size(); ++i)
        {
            std::string light = "spotLights[" + std::to_string(i) + "]";
            gBasicShader->SetUniform3f(light + ".base.base.colour", gSpotLights[i]->GetColour());
            gBasicShader->SetUniform1f(light + ".base.base.ambientIntensity", gSpotLights[i]->GetAmbientIntensity());
            gBasicShader->SetUniform1f(light + ".base.base.diffuseIntensity", gSpotLights[i]->GetDiffuseIntensity());
            gBasicShader->SetUniform3f(light + ".base.position", gSpotLights[i]->GetPosition());
            gBasicShader->SetUniform1f(light + ".base.constant", gSpotLights[i]->GetConstant());
            gBasicShader->SetUniform1f(light + ".base.linear", gSpotLights[i]->GetLinear());
            gBasicShader->SetUniform1f(light + ".base.exponent", gSpotLights[i]->GetExponent());
            gBasicShader->SetUniform3f(light + ".direction", gSpotLights[i]->GetDirection());
            gBasicShader->SetUniform1f(light + ".edgeCosine", gSpotLights[i]->GetEdgeCosine());

            gSpotLights[i]->GetShadowMap()->GetMap()->Bind();

            std::string shadowMap = "omniShadowMaps[" + std::to_string(gPointLights.size() + i) + "]";
            gBasicShader->SetUniform1i(shadowMap + ".map", gSpotLights[i]->GetShadowMap()->GetMap()->GetUnit());
        }

        gBasicShader->SetUniform1f("farPlane", Renderer::FAR_PLANE);

        DrawModels(gBasicShader, true);
    }

    gWindow->SwapBuffers();
    gWindow->ClearMousePosDeltas();

    return true;
}
