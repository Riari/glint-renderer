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
#include "Asset/Asset.h"
#include "Asset/Manager.h"
#include "Renderer/GL/ShaderProgram.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/Light.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Camera.h"
#include "Renderer/Map.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/Model.h"
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
Renderer::GL::ShaderProgram* gTestShader;

const std::vector<GLfloat> PYRAMID_VERTICES = {
    // X, Y, Z,             U, V,           NX, NY, NZ
    -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 1.0f,      0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, -0.6f,     1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,       0.5f, 1.0f,     0.0f, 0.0f, 0.0f,
};

const std::vector<GLuint> PYRAMID_INDICES = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2,
};

const std::vector<GLfloat> FLOOR_VERTICES = {
    -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
    10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
    10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0f, -1.0f, 0.0f,
};

const std::vector<GLuint> FLOOR_INDICES = {
    0, 2, 1,
    1, 2, 3,
};

std::vector<Renderer::Model*> gTestModels;

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

    delete gTestShader;

    for (auto model : gTestModels)
    {
        delete model;
    }
    gTestModels.clear();

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
        gWindow = new App::Window(1920, 1080, "udemy-opengl");
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
        std::string vertSource = Util::File::Read("shaders/basic-material.vert.glsl");
        std::string fragSource = Util::File::Read("shaders/basic-material.frag.glsl");
        gTestShader = new Renderer::GL::ShaderProgram();
        if (gTestShader->Build(vertSource, fragSource) != 0)
        {
            glfwTerminate();
            return false;
        }

        gTestShader->Use();
    }

    spdlog::info("Registering assets");
    {
        stbi_set_flip_vertically_on_load(true);

        Asset::Manager::RegisterInitialiser(".jpg", Asset::Type::InitialiseImage);
        Asset::Manager::RegisterInitialiser(".png", Asset::Type::InitialiseImage);
        Asset::Manager::RegisterLoader("Image", Asset::Type::LoadImage);
        Asset::Manager::RegisterLoader("Model", Asset::Type::ModelLoader::Load);

        Asset::Manager::Initialise();
        Asset::Manager::LoadAssets();
    }

    spdlog::info("Creating world objects");
    {
        gCamera = new Renderer::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
        gDirectionalLight = new Renderer::DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.1f, glm::vec3(0.0f, 0.0f, -1.0f));

        // gPointLights.push_back(new Renderer::PointLight(glm::vec3(1.0f, 0.0f, 0.5f), 0.0f, 1.0f, glm::vec3(0.0f, 0.5f, -3.0f), 0.3f, 0.1f, 0.1f));
        // gPointLights.push_back(new Renderer::PointLight(glm::vec3(0.0f, 0.75f, 1.0f), 0.0f, 1.0f, glm::vec3(3.0f, 0.5f, 0.0f), 0.3f, 0.1f, 0.1f));
        // gPointLights.push_back(new Renderer::PointLight(glm::vec3(1.0f, 0.42f, 0.0f), 0.0f, 1.0f, glm::vec3(-3.0f, 0.5f, 0.0f), 0.3f, 0.1f, 0.1f));
        // gPointLights.push_back(new Renderer::PointLight(glm::vec3(0.47f, 0.0f, 1.0f), 0.0f, 1.0f, glm::vec3(0.0f, 0.5f, 3.0f), 0.3f, 0.1f, 0.1f));

        gSpotLights.push_back(new Renderer::SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 2.0f, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.1f, 0.1f, 40.0f));

        gTestModels.push_back(new Renderer::Model());
        gTestModels.push_back(new Renderer::Model());
        gTestModels.push_back(new Renderer::Model());
        gTestModels.push_back(new Renderer::Model());

        glActiveTexture(GL_TEXTURE0);

        auto mapBrick = new Renderer::Map(Asset::Manager::Get<Asset::Type::Image>("Image::Brick"));
        auto mapDirt = new Renderer::Map(Asset::Manager::Get<Asset::Type::Image>("Image::Dirt"));
        auto mapStainlessSteel = new Renderer::Map(Asset::Manager::Get<Asset::Type::Image>("Image::StainlessSteel"));

        gTestModels[0]->AddMesh(
            new Renderer::Mesh(PYRAMID_VERTICES, PYRAMID_INDICES, 8, { 3, 2, 3 }, true),
            new Renderer::Material(mapBrick, 1.0f, 32.0f));
        gTestModels[0]->SetPosition(glm::vec3(-1.0f, 2.0f, -2.5f));
        gTestModels[0]->SetScale(glm::vec3(0.5f));

        gTestModels[1]->AddMesh(
            new Renderer::Mesh(PYRAMID_VERTICES, PYRAMID_INDICES, 8, { 3, 2, 3 }, true),
            new Renderer::Material(mapDirt, 0.3f, 4.0f));
        gTestModels[1]->SetPosition(glm::vec3(1.0f, 2.0f, -2.5f));
        gTestModels[1]->SetScale(glm::vec3(0.65f));

        gTestModels[2]->AddMesh(
            new Renderer::Mesh(FLOOR_VERTICES, FLOOR_INDICES, 8, { 3, 2, 3 }, false),
            new Renderer::Material(mapStainlessSteel, 2.0f, 128.0f));
        gTestModels[2]->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));

        // TODO: Define a better way of converting imported models and materials into renderer-ready models
        const Asset::Type::Model& model = Asset::Manager::Get<Asset::Type::Model>("Model::F1Car");
        for (size_t i = 0; i < model.mMaterials.size(); ++i)
        {
            Asset::Type::MaterialData* material = model.mMaterials[i];

            if (!material->mHasTexture)
            {
                Renderer::Map* map = new Renderer::Map(glm::vec3(1.0f, 0.0f, 1.0f));
                gTestModels[3]->AddMaterial(new Renderer::Material(map, material->mSpecularIntensity, material->mShininess));
            }
            else
            {
                const Asset::Type::Image& image = Asset::Manager::Get<Asset::Type::Image>(material->mTextureAssetName);
                gTestModels[3]->AddMaterial(new Renderer::Material(new Renderer::Map(image), material->mSpecularIntensity, material->mShininess));
            }
        }

        for (size_t i = 0; i < model.mMeshes.size(); ++i)
        {
            Asset::Type::MeshData* mesh = model.mMeshes[i];

            gTestModels[3]->AddMesh(
                new Renderer::Mesh(mesh->mVertices, mesh->mIndices, mesh->mVertexLength, mesh->mAttributeSizes, false),
                mesh->mMaterialIndex);
        }

        gTestModels[3]->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
        gTestModels[3]->SetScale(glm::vec3(0.01f));
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

    // Update
    glfwPollEvents();

    glm::mat4 projection = glm::perspective(45.0f, (float)gWindow->GetWidth() / (float)gWindow->GetHeight(), 0.1f, 100.0f);

    // Draw
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gCamera->Update(gWindow, static_cast<float>(gDeltaTime));

    // TODO: Come up with a better way of managing the interface between world objects (e.g. lights) and shader programs
    // TODO: Validate shader inputs (e.g. point light count must not exceed MAX_POINT_LIGHTS)

    gTestShader->Use();
    glm::mat4 view = gCamera->GetViewMatrix();
    gTestShader->SetUniformMatrix4fv("view", glm::value_ptr(view));
    gTestShader->SetUniformMatrix4fv("projection", glm::value_ptr(projection));
    gTestShader->SetUniform3f("directionalLight.base.colour", gDirectionalLight->GetColour());
    gTestShader->SetUniform1f("directionalLight.base.ambientIntensity", gDirectionalLight->GetAmbientIntensity());
    gTestShader->SetUniform1f("directionalLight.base.diffuseIntensity", gDirectionalLight->GetDiffuseIntensity());
    gTestShader->SetUniform3f("directionalLight.direction", gDirectionalLight->GetDirection());
    gTestShader->SetUniform3f("eyePosition", gCamera->GetPosition());

    gTestShader->SetUniform1i("pointLightCount", gPointLights.size());

    for (size_t i = 0; i < gPointLights.size(); ++i)
    {
        std::string light = "pointLights[" + std::to_string(i) + "]";
        gTestShader->SetUniform3f(light + ".base.colour", gPointLights[i]->GetColour());
        gTestShader->SetUniform1f(light + ".base.ambientIntensity", gPointLights[i]->GetAmbientIntensity());
        gTestShader->SetUniform1f(light + ".base.diffuseIntensity", gPointLights[i]->GetDiffuseIntensity());
        gTestShader->SetUniform3f(light + ".position", gPointLights[i]->GetPosition());
        gTestShader->SetUniform1f(light + ".constant", gPointLights[i]->GetConstant());
        gTestShader->SetUniform1f(light + ".linear", gPointLights[i]->GetLinear());
        gTestShader->SetUniform1f(light + ".exponent", gPointLights[i]->GetExponent());
    }

    gTestShader->SetUniform1i("spotLightCount", gSpotLights.size());

    for (size_t i = 0; i < gSpotLights.size(); ++i)
    {
        std::string light = "spotLights[" + std::to_string(i) + "]";
        gTestShader->SetUniform3f(light + ".base.base.colour", gSpotLights[i]->GetColour());
        gTestShader->SetUniform1f(light + ".base.base.ambientIntensity", gSpotLights[i]->GetAmbientIntensity());
        gTestShader->SetUniform1f(light + ".base.base.diffuseIntensity", gSpotLights[i]->GetDiffuseIntensity());
        gTestShader->SetUniform3f(light + ".base.position", gSpotLights[i]->GetPosition());
        gTestShader->SetUniform1f(light + ".base.constant", gSpotLights[i]->GetConstant());
        gTestShader->SetUniform1f(light + ".base.linear", gSpotLights[i]->GetLinear());
        gTestShader->SetUniform1f(light + ".base.exponent", gSpotLights[i]->GetExponent());
        gTestShader->SetUniform3f(light + ".direction", gSpotLights[i]->GetDirection());
        gTestShader->SetUniform1f(light + ".edgeCosine", gSpotLights[i]->GetEdgeCosine());
    }

    for (size_t i = 0; i < gTestModels.size(); ++i)
    {
        Renderer::Model* model = gTestModels[i];

        gTestShader->SetUniformMatrix4fv("model", glm::value_ptr(model->GetModelMatrix()));

        std::vector<Renderer::Mesh*> meshes = model->GetMeshes();
        std::vector<Renderer::Material*> materials = model->GetMaterials();
        std::vector<size_t> meshToMaterial = model->GetMeshToMaterialIndices();

        for (size_t meshIndex = 0; meshIndex < meshToMaterial.size(); ++meshIndex)
        {
            size_t materialIndex = meshToMaterial[meshIndex];
            Renderer::Material* material = materials[materialIndex];
            Renderer::Mesh* mesh = meshes[meshIndex];

            if (material->GetBaseMap()->GetType() == Renderer::MapType::COLOUR)
            {
                gTestShader->SetUniform1i("material.useTexture", 0);
                gTestShader->SetUniform3f("material.baseColour", material->GetBaseMap()->GetColour());
            }
            else
            {
                gTestShader->SetUniform1i("material.useTexture", 1);
            }

            gTestShader->SetUniform1f("material.specularIntensity", material->GetSpecularIntensity());
            gTestShader->SetUniform1f("material.shininess", material->GetShininess());

            material->Bind();
            mesh->Bind();
            mesh->Draw();
            mesh->Unbind();
            material->Unbind();
        }
    }

    gWindow->SwapBuffers();
    gWindow->ClearMousePosDeltas();

    return true;
}
