#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "App/Window.h"
#include "GL/Mesh.h"
#include "GL/ShaderProgram.h"
#include "GL/Texture.h"
#include "Util/File.h"
#include "Util/GL.h"
#include "Util/Math.h"

bool init();
bool loop();

App::Window* gWindow;

GL::ShaderProgram *gTestShader;

const std::vector<GLfloat> PYRAMID_VERTICES = {
    -1.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

const std::vector<GLuint> PYRAMID_INDICES = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2,
};

std::vector<GL::Mesh*> gTestMeshes;

float triRotation = 0.0f;

int gFramesPerSecond;

int main()
{
    if (!init())
    {
        SPDLOG_ERROR("Initialisation failed");
        return -1;
    }

    double previousTime = glfwGetTime();
    int frameCount{0};

    while (true)
    {
        if (!loop()) break;

        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - previousTime >= 1.0)
        {
            gFramesPerSecond = frameCount;
            frameCount = 0;
            previousTime = currentTime;
        }
    }

    delete gWindow;
    delete gTestShader;
    for (auto mesh : gTestMeshes)
    {
        delete mesh;
    }
    gTestMeshes.clear();

    glfwTerminate();

    return 0;
}

static void error_callback(int error, const char* description)
{
    SPDLOG_ERROR("Error {}: {}", error, description);
}

bool init()
{
    SPDLOG_INFO("Initialising GLFW + GLEW with OpenGL 4.1");

    // GLFW initialisation
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        return false;
    }

    // Window creation
    {
        gWindow = new App::Window(1920, 1080, "udemy-opengl");
        gWindow->MakeCurrent();
    }

    // GLEW initialisation
    if (glewInit() != GLEW_OK)
    {
        return false;
    }

    // Shader initialisation
    {
        std::string vertSource = Util::File::Read("shaders/tri.vert.glsl");
        std::string fragSource = Util::File::Read("shaders/tri.frag.glsl");
        gTestShader = new GL::ShaderProgram(vertSource.c_str(), fragSource.c_str());
        if (gTestShader->Build() != 0)
        {
            SPDLOG_ERROR("Shader program failed to build");
            glfwTerminate();
            return false;
        }

        gTestShader->Use();
    }

    // Mesh initialisation
    {
        gTestMeshes.push_back(new GL::Mesh(PYRAMID_VERTICES, PYRAMID_INDICES));
        gTestMeshes.push_back(new GL::Mesh(PYRAMID_VERTICES, PYRAMID_INDICES));

        gTestMeshes[0]->SetPosition(glm::vec3(-1.0f, 0.0f, -2.5f));
        gTestMeshes[0]->SetScale(glm::vec3(0.5f));
        gTestMeshes[1]->SetPosition(glm::vec3(1.0f, 0.0f, -2.5f));
        gTestMeshes[1]->SetScale(glm::vec3(0.65f));
    }

    // OpenGL flags
    {
        glEnable(GL_DEPTH_TEST);
    }

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

    triRotation += 0.01f;
    if (triRotation > 360.0f)
    {
        triRotation = -360.0f;
    }

    glm::mat4 view(1.0f);

    glm::mat4 projection = glm::perspective(45.0f, (float)gWindow->GetWidth() / (float)gWindow->GetHeight(), 0.1f, 100.0f);

    // Draw
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gTestShader->Use();
    gTestShader->SetUniformMatrix4fv("view", glm::value_ptr(view));
    gTestShader->SetUniformMatrix4fv("projection", glm::value_ptr(projection));

    for (auto pTestMesh : gTestMeshes)
    {
        pTestMesh->SetRotation(glm::vec3(0.0f, triRotation, 0.0f));
        gTestShader->SetUniformMatrix4fv("model", glm::value_ptr(pTestMesh->GetModelMatrix()));
        pTestMesh->Draw();
    }

    gWindow->SwapBuffers();
    gWindow->ClearMousePosChanges();

    return true;
}
