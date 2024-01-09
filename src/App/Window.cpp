#include "Window.h"

namespace App
{
    Window::Window(int width, int height, const char* title)
        : mWidth(width)
        , mHeight(height)
        , mTitle(title)
    {
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        mHandle = glfwCreateWindow(width, height, title, NULL, NULL);
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
};
