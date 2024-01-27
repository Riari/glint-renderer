#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "App/Window.h"
#include "GL/ShaderProgram.h"
#include "GL/Texture.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/Light.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Camera.h"
#include "Renderer/Map.h"
#include "Renderer/Material.h"
#include "Renderer/Model.h"
#include "Util/File.h"
#include "Util/GL.h"

bool init();
bool loop();

App::Window* gWindow;
Renderer::Camera* gCamera;
Renderer::DirectionalLight* gDirectionalLight;
std::vector<Renderer::PointLight*> gPointLights;
GL::ShaderProgram* gTestShader;

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
    SPDLOG_INFO("Beginning initialisation...");
    if (!init())
    {
        SPDLOG_ERROR("Initialisation failed");
        return -1;
    }

    double previousTime = glfwGetTime();
    double previousFPSUpdateTime = previousTime;
    int frameCount{0};

    SPDLOG_INFO("Entering main loop...");
    while (true)
    {
        if (!loop()) break;

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

    SPDLOG_INFO("Cleaning up...");

    delete gWindow;
    delete gCamera;
    delete gDirectionalLight;

    for (auto pointLight : gPointLights)
    {
        delete pointLight;
    }
    gPointLights.clear();

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
    SPDLOG_ERROR("Error {}: {}", error, description);
}

bool init()
{
    SPDLOG_INFO("Initialising GLFW");

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

    SPDLOG_INFO("Initialising GLEW");

    // GLEW
    if (glewInit() != GLEW_OK)
    {
        return false;
    }

    SPDLOG_INFO("Setting OpenGL flags");

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

    SPDLOG_INFO("Building shaders");
    {
        std::string vertSource = Util::File::Read("shaders/basic-material.vert.glsl");
        std::string fragSource = Util::File::Read("shaders/basic-material.frag.glsl");
        gTestShader = new GL::ShaderProgram(vertSource.c_str(), fragSource.c_str());
        if (gTestShader->Build() != 0)
        {
            glfwTerminate();
            return false;
        }

        gTestShader->Use();
    }

    // TODO: Implement asset management for loading and sharing things like textures

    SPDLOG_INFO("Creating world objects");
    {
        gCamera = new Renderer::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.5f);
        gDirectionalLight = new Renderer::DirectionalLight(glm::vec3(0.8f, 0.8f, 1.0f), 0.5f, 0.5f, glm::vec3(0.0f, -1.0f, -0.0f));

        gPointLights.push_back(new Renderer::PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f, glm::vec3(0, 1.0f, -5.0f), 0.3f, 0.1f, 0.1f));
        gPointLights.push_back(new Renderer::PointLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f, glm::vec3(2.5f, 1.0f, -2.0f), 0.3f, 0.1f, 0.1f));
        gPointLights.push_back(new Renderer::PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f, glm::vec3(-2.5f, 1.0f, -2.0f), 0.3f, 0.1f, 0.1f));

        gTestModels.push_back(new Renderer::Model(PYRAMID_VERTICES, PYRAMID_INDICES, 8, { 3, 2, 3 }, true));
        gTestModels.push_back(new Renderer::Model(PYRAMID_VERTICES, PYRAMID_INDICES, 8, { 3, 2, 3 }, true));
        gTestModels.push_back(new Renderer::Model(FLOOR_VERTICES, FLOOR_INDICES, 8, { 3, 2, 3 }, false));

        // TODO: These are temporary calls for the textures that will be loaded for the materials below
        stbi_set_flip_vertically_on_load(true);
        glActiveTexture(GL_TEXTURE0);

        gTestModels[0]->SetMaterial(new Renderer::Material(new Renderer::Map("assets/textures/brick.png"), 1.0f, 32.0f));
        gTestModels[0]->SetPosition(glm::vec3(-1.0f, 0.0f, -2.5f));
        gTestModels[0]->SetScale(glm::vec3(0.5f));

        gTestModels[1]->SetMaterial(new Renderer::Material(new Renderer::Map("assets/textures/dirt.png"), 0.3f, 4.0f));
        gTestModels[1]->SetPosition(glm::vec3(1.0f, 0.0f, -2.5f));
        gTestModels[1]->SetScale(glm::vec3(0.65f));

        gTestModels[2]->SetMaterial(new Renderer::Material(new Renderer::Map(glm::vec3(0.2f, 0.2f, 0.2f)), 0.3f, 4.0f));
        gTestModels[2]->SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));
    }

    SPDLOG_INFO("Initialisation complete!");

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

    for (size_t i = 0; i < gTestModels.size(); ++i)
    {
        Renderer::Model* model = gTestModels[i];
        Renderer::Material* material = model->GetMaterial();

        gTestShader->SetUniformMatrix4fv("model", glm::value_ptr(model->GetModelMatrix()));

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

        model->Bind();
        model->Draw();
        model->Unbind();
    }

    gWindow->SwapBuffers();
    gWindow->ClearMousePosDeltas();

    return true;
}
