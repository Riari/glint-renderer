#include "Window.h"

#include <spdlog/spdlog.h>

namespace App
{
    const int GL_VERSION_MAJOR = 4;
    const int GL_VERSION_MINOR = 1;

    Window::Window(int width, int height, const char* title)
        : mWidth(width)
        , mHeight(height)
        , mTitle(title)
    {
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        SPDLOG_INFO("Creating window {} x {} with OpenGL {}.{} context", width, height, GL_VERSION_MAJOR, GL_VERSION_MINOR);

        mHandle = glfwCreateWindow(width, height, title, NULL, NULL);

        glfwSetWindowUserPointer(mHandle, this);

        InitCallbacks();
        glfwSetInputMode(mHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(mHandle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

        for (size_t i = 0; i < MAX_KEYS; ++i)
        {
            mKeysPressed[i] = false;
        }
    }

    Window::~Window()
    {
        glfwDestroyWindow(mHandle);
    }

    int Window::GetWidth() const
    {
        return mWidth;
    }

    int Window::GetHeight() const
    {
        return mHeight;
    }

    void Window::MakeCurrent() const
    {
        glfwMakeContextCurrent(mHandle);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(mHandle);
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(mHandle);
    }

    void Window::ClearMousePosDeltas()
    {
        if (mMouseDeltaX == 0.0f && mMouseDeltaY == 0.0f)
        {
            return;
        }

        mMouseDeltaX = 0.0f;
        mMouseDeltaY = 0.0f;
    }

    bool Window::IsKeyPressed(int key) const
    {
        return mKeysPressed[key];
    }

    float Window::GetMouseDeltaX() const
    {
        return mMouseDeltaX;
    }

    float Window::GetMouseDeltaY() const
    {
        return mMouseDeltaY;
    }

    void Window::InitCallbacks() const
    {
        glfwSetKeyCallback(mHandle, KeyCallback);
        glfwSetCursorPosCallback(mHandle, MousePositionCallback);
    }

    Window* Window::GetCallbackHandle(GLFWwindow* handle)
    {
        return static_cast<Window*>(glfwGetWindowUserPointer(handle));
    }

    void Window::KeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods)
    {
        Window* window = GetCallbackHandle(handle);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(handle, GL_TRUE);
            return;
        }

        window->mKeysPressed[key] = action != GLFW_RELEASE;
    }

    void Window::MousePositionCallback(GLFWwindow* handle, double x, double y)
    {
        Window* window = GetCallbackHandle(handle);

        if (window->mMouseFirstMoved)
        {
            window->mMouseLastX = x;
            window->mMouseLastY = y;
            window->mMouseFirstMoved = false;
        }

        window->mMouseDeltaX = x - window->mMouseLastX;
        window->mMouseDeltaY = window->mMouseLastY - y;
        window->mMouseLastX = x;
        window->mMouseLastY = y;
    }
};
