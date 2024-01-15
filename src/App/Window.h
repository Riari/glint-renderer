#pragma once

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>

namespace App
{
    const size_t MAX_KEYS = 1024;

    class Window
    {
    public:
        bool mMouseFirstMoved{false};

        Window(int width, int height, const char* title);
        ~Window();

        int GetWidth() const;
        int GetHeight() const;

        void MakeCurrent() const;
        bool ShouldClose() const;
        void SwapBuffers() const;
        void ClearMousePosDeltas();

        bool IsKeyPressed(int key) const;
        float GetMouseDeltaX() const;
        float GetMouseDeltaY() const;

    private:
        GLFWwindow* mHandle;

        int mWidth, mHeight;
        const char* mTitle;

        bool mKeysPressed[MAX_KEYS];

        float mMouseLastX{0.f}, mMouseLastY{0.f}, mMouseDeltaX{0.f}, mMouseDeltaY{0.f};

        void InitCallbacks() const;

        static Window* GetCallbackHandle(GLFWwindow* handle);

        static void KeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods);
        static void MousePositionCallback(GLFWwindow* handle, double x, double y);
    };
};
