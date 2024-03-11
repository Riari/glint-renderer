#include <vector>

#include <assimp/Importer.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
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
#include "Renderer/Context.h"
#include "Renderer/Map.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/Model.h"
#include "Renderer/Pipeline/Pass/ClearPass.h"
#include "Renderer/Pipeline/Pass/ScenePass.h"
#include "Renderer/Pipeline/Pass/ShadowMapPass.h"
#include "Renderer/Pipeline/Pass/SkyboxPass.h"
#include "Renderer/Pipeline/Pipeline.h"
#include "Renderer/Scene.h"
#include "Renderer/Skybox.h"
#include "Util/GL.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool init();
bool loop();

App::Window* gWindow;
Renderer::GL::Shader::Program* gDebugQuadShader;
Renderer::Context* gRendererContext;
Renderer::Scene* gScene;
Renderer::Pipeline* gPipeline;

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
    delete gDebugQuadShader;
    delete gScene;
    delete gDebugQuad;

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

    spdlog::info("Initialising renderer pipeline");
    {
        gPipeline = new Renderer::Pipeline();
        gPipeline->AddPass(new Renderer::ShadowMapPass());
        gPipeline->AddPass(new Renderer::ClearPass());
        gPipeline->AddPass(new Renderer::SkyboxPass());
        gPipeline->AddPass(new Renderer::ScenePass());

        if (!gPipeline->Init())
        {
            glfwTerminate();
            spdlog::error("Failed to initialise pipeline");
            return false;
        }
    }

    gRendererContext = new Renderer::Context(*gWindow);
    gScene = new Renderer::Scene();

    spdlog::info("Creating world objects");
    {
        gScene->SetCamera(new Renderer::Camera(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            -60.0f, 0.0f,
            1.0f,
            0.3f));

        gScene->SetDirectionalLight(new Renderer::DirectionalLight(
            glm::vec3(1.0f, 0.6f, 0.4f),
            0.2f,
            1.5f,
            glm::vec3(0.0f, -25.0f, -10.0f),
            5));

        // gScene->AddPointLight(new Renderer::PointLight(
        //     glm::vec3(1.0f, 0.0f, 0.5f),
        //     0.1f,
        //     2.0f,
        //     glm::vec3(0.0f, 2.5f, -3.0f),
        //     0.3f,
        //     0.1f,
        //     0.1f,
        //     6));
        // gScene->AddPointLight(new Renderer::PointLight(
        //     glm::vec3(0.0f, 0.75f, 1.0f),
        //     0.1f,
        //     2.0f,
        //     glm::vec3(3.0f, 2.5f, 0.0f),
        //     0.3f,
        //     0.1f,
        //     0.1f,
        //     7));
        // gScene->AddPointLight(new Renderer::PointLight(
        //     glm::vec3(1.0f, 0.42f, 0.0f),
        //     0.1f,
        //     2.0f,
        //     glm::vec3(-3.0f, 2.5f, 0.0f),
        //     0.3f,
        //     0.1f,
        //     0.1f,
        //     8));
        // gScene->AddPointLight(new Renderer::PointLight(
        //     glm::vec3(0.47f, 0.0f, 1.0f),
        //     0.1f,
        //     2.0f,
        //     glm::vec3(0.0f, 2.5f, 3.0f),
        //     0.3f,
        //     0.1f,
        //     0.1f,
        //     9));

        // gScene->AddSpotLight(new Renderer::SpotLight(
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
        // gScene->AddSpotLight(new Renderer::SpotLight(
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

        std::vector<Renderer::Model*> models;

        models.push_back(new Renderer::Model());
        models.push_back(new Renderer::Model());
        models.push_back(new Renderer::Model());
        models.push_back(new Renderer::Model());

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

        models[0]->AddMesh(
            new Renderer::Mesh(Renderer::PRIMITIVE_PLANE_VERTICES, Renderer::PRIMITIVE_PLANE_INDICES, 8, { 3, 2, 3 }, false),
            materialStainlessSteel);
        models[0]->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));

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

            models[1]->AddMaterial(material);
        }

        for (size_t i = 0; i < model.mMeshes.size(); ++i)
        {
            Asset::Type::MeshData* mesh = model.mMeshes[i];

            models[1]->AddMesh(
                new Renderer::Mesh(mesh->mVertices, mesh->mIndices, mesh->mVertexLength, mesh->mVertexLayout, false),
                mesh->mMaterialIndex);
        }

        models[1]->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
        models[1]->SetScale(glm::vec3(0.01f));

        gScene->AddModels(models);

        gScene->SetSkybox(new Renderer::Skybox("Skybox::yellowcloud"));
    }

    spdlog::info("Initialisation complete!");

    return true;
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

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)gWindow->GetWidth() / (float)gWindow->GetHeight(), 0.1f, 100.0f);
    gScene->GetCamera()->Update(gWindow, static_cast<float>(gDeltaTime));
    glm::mat4 view = gScene->GetCamera()->GetViewMatrix();

    Renderer::DirectionalLight* directionalLight = gScene->GetDirectionalLight();

    if (gWindow->IsKeyPressed(GLFW_KEY_UP))
    {
        glm::vec3 direction = directionalLight->GetDirection();
        direction.y += 0.1f;
        directionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_DOWN))
    {
        glm::vec3 direction = directionalLight->GetDirection();
        direction.y -= 0.1f;
        directionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_LEFT))
    {
        glm::vec3 direction = directionalLight->GetDirection();
        direction.x -= 0.1f;
        directionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_RIGHT))
    {
        glm::vec3 direction = directionalLight->GetDirection();
        direction.x += 0.1f;
        directionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_O))
    {
        glm::vec3 direction = directionalLight->GetDirection();
        direction.z += 0.1f;
        directionalLight->SetDirection(direction);
    }

    if (gWindow->IsKeyPressed(GLFW_KEY_L))
    {
        glm::vec3 direction = directionalLight->GetDirection();
        direction.z -= 0.1f;
        directionalLight->SetDirection(direction);
    }

    gPipeline->Execute(gRendererContext, gScene);

    gWindow->SwapBuffers();
    gWindow->ClearMousePosDeltas();

    return true;
}
