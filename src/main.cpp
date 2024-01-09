#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "GL/Buffer/EBO.h"
#include "GL/Buffer/VAO.h"
#include "GL/Buffer/VBO.h"
#include "GL/ShaderProgram.h"
#include "GL/Texture.h"
#include "Util/File.h"
#include "Util/GL.h"
#include "Util/Math.h"

bool init();
bool loop();

const GLuint WINDOW_WIDTH{1920}, WINDOW_HEIGHT{1080};
GLFWwindow *pWindow;

const GLfloat PYRAMIC_VERTICES[] = {
    -1.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

const unsigned int PYRAMIC_INDICES[] = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2,
};

GL::ShaderProgram *pTriShader;

float triOffset = 0.0f;
float triMaxOffset = 0.5f;
float triIncrement = 0.005f;
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

    glfwDestroyWindow(pWindow);
    glfwTerminate();

    return 0;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

bool init()
{
    SPDLOG_INFO("Initialising GLFW context with OpenGL 4.1");

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "udemy-opengl", NULL, NULL);
    glfwMakeContextCurrent(pWindow);

    if (glewInit() != GLEW_OK)
    {
        return false;
    }

    GL::VAO vao;
    GL::VBO vbo;
    GL::EBO ebo;

    vao.Bind();

    vbo.Bind();
    vbo.Write(sizeof(PYRAMIC_VERTICES), PYRAMIC_VERTICES, GL_STATIC_DRAW);

    vao.SetAttributePointer(0, 3, 3 * sizeof(float), (void*)0);
    vao.EnableAttributePointer(0);

    ebo.Bind();
    ebo.Write(sizeof(PYRAMIC_INDICES), PYRAMIC_INDICES, GL_STATIC_DRAW);

    std::string vertSource = Util::File::Read("shaders/tri.vert.glsl");
    std::string fragSource = Util::File::Read("shaders/tri.frag.glsl");
    pTriShader = new GL::ShaderProgram(vertSource.c_str(), fragSource.c_str());
    if (pTriShader->Build() != 0)
    {
        SPDLOG_ERROR("Shader program failed to build");
        glfwTerminate();
        return false;
    }

    pTriShader->Use();

    glEnable(GL_DEPTH_TEST);

    return true;
}

bool loop()
{
    if (glfwWindowShouldClose(pWindow))
    {
        return false;
    }

    // Update
    glfwPollEvents();

    if (triOffset > triMaxOffset || triOffset < -triMaxOffset)
    {
        triIncrement = -triIncrement;
    }

    triOffset += triIncrement;

    triRotation += 0.1f;
    if (triRotation > 360.0f)
    {
        triRotation = 360.0f;
    }

    glm::mat4 model(1.0f);
    // model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
    model = glm::rotate(model, triRotation * Util::Math::DEG_TO_RAD, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

    // Draw
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pTriShader->Use();
    pTriShader->SetUniformMatrix4fv("model", glm::value_ptr(model));

    // glDrawArrays(GL_TRIANGLES, 0, 3); // for drawing without the EBO (indexed drawing)
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(pWindow);

    return true;
}
