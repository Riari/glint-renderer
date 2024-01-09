#pragma once

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>

namespace App
{
    class Window
    {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        int GetWidth() const;
        int GetHeight() const;

        void MakeCurrent() const;
        bool ShouldClose() const;
        void SwapBuffers() const;

    private:
        GLFWwindow* mHandle;

        int mWidth, mHeight;
        const char* mTitle;
    };
};
