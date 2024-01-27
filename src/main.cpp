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
#include "World/Camera.h"
#include "World/DirectionalLight.h"
#include "World/Light.h"
#include "World/Material.h"
#include "World/Mesh.h"
#include "Util/File.h"
#include "Util/GL.h"

bool init();
bool loop();

App::Window* gWindow;
World::Camera* gCamera;
World::Light* gAmbientLight;
World::DirectionalLight* gDirectionalLight;
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

std::vector<World::Material*> gTestMaterials;
std::vector<World::Mesh*> gTestMeshes;
std::vector<GL::Texture*> gTestTextures;

float triRotation = 0.0f;

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
    delete gAmbientLight;
    delete gDirectionalLight;
    delete gTestShader;

    for (auto material : gTestMaterials)
    {
        delete material;
    }
    gTestMaterials.clear();

    for (auto mesh : gTestMeshes)
    {
        delete mesh;
    }
    gTestMeshes.clear();

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

    SPDLOG_INFO("Creating world objects");
    {
        gCamera = new World::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.5f);
        gAmbientLight = new World::Light(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
        gDirectionalLight = new World::DirectionalLight(glm::vec3(0.8f, 0.8f, 1.0f), 0.5f, glm::vec3(0.0f, -1.0f, -0.0f));

        gTestMaterials.push_back(new World::Material(1.0f, 32.0f));
        gTestMaterials.push_back(new World::Material(0.3f, 4.0f));

        gTestMeshes.push_back(new World::Mesh(PYRAMID_VERTICES, PYRAMID_INDICES, 8, { 3, 2, 3 }));
        gTestMeshes.push_back(new World::Mesh(PYRAMID_VERTICES, PYRAMID_INDICES, 8, { 3, 2, 3 }));

        gTestMeshes[0]->SetMaterial(gTestMaterials[0]);
        gTestMeshes[0]->SetPosition(glm::vec3(-1.0f, 0.0f, -2.5f));
        gTestMeshes[0]->SetScale(glm::vec3(0.5f));
        gTestMeshes[1]->SetMaterial(gTestMaterials[1]);
        gTestMeshes[1]->SetPosition(glm::vec3(1.0f, 0.0f, -2.5f));
        gTestMeshes[1]->SetScale(glm::vec3(0.65f));
    }

    SPDLOG_INFO("Creating test textures");
    {
        stbi_set_flip_vertically_on_load(true);
        glActiveTexture(GL_TEXTURE0);
        gTestTextures.push_back(new GL::Texture("assets/textures/brick.png"));
        gTestTextures.push_back(new GL::Texture("assets/textures/dirt.png"));
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

    // triRotation += 0.01f;
    // if (triRotation > 360.0f)
    // {
    //     triRotation = -360.0f;
    // }

    glm::mat4 projection = glm::perspective(45.0f, (float)gWindow->GetWidth() / (float)gWindow->GetHeight(), 0.1f, 100.0f);

    // Draw
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gCamera->Update(gWindow, static_cast<float>(gDeltaTime));

    gTestShader->Use();
    glm::mat4 view = gCamera->GetViewMatrix();
    gTestShader->SetUniformMatrix4fv("view", glm::value_ptr(view));
    gTestShader->SetUniformMatrix4fv("projection", glm::value_ptr(projection));
    gTestShader->SetUniform3f("ambientLight.colour", gAmbientLight->GetColour());
    gTestShader->SetUniform1f("ambientLight.intensity", gAmbientLight->GetIntensity());
    gTestShader->SetUniform3f("directionalLight.colour", gDirectionalLight->GetColour());
    gTestShader->SetUniform1f("directionalLight.intensity", gDirectionalLight->GetIntensity());
    gTestShader->SetUniform3f("directionalLight.direction", gDirectionalLight->GetDirection());
    gTestShader->SetUniform3f("eyePosition", gCamera->GetPosition());

    for (size_t i = 0; i < gTestTextures.size(); ++i)
    {
        gTestMeshes[i]->SetRotation(glm::vec3(0.0f, triRotation, 0.0f));
        gTestShader->SetUniformMatrix4fv("model", glm::value_ptr(gTestMeshes[i]->GetModelMatrix()));
        gTestShader->SetUniform1f("material.specularIntensity", gTestMeshes[i]->GetMaterial()->GetSpecularIntensity());
        gTestShader->SetUniform1f("material.shininess", gTestMeshes[i]->GetMaterial()->GetShininess());
        gTestTextures[i]->Bind();
        gTestMeshes[i]->Draw();
    }

    gWindow->SwapBuffers();
    gWindow->ClearMousePosDeltas();

    return true;
}
