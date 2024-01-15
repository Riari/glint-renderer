#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"

namespace App
{
    class Camera
    {
    public:
        Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float moveSpeed, float turnSpeed);
        ~Camera();

        void Update(Window* pWindow, float deltaTime);

        glm::mat4 GetViewMatrix();

    private:
        glm::vec3 mPosition;
        glm::vec3 mFront;
        glm::vec3 mRight;
        glm::vec3 mUp;
        glm::vec3 mWorldUp;

        float mYaw;
        float mPitch;

        float mMoveSpeed;
        float mTurnSpeed;

        // UpdateVectors updates the vectors (mFront, mRight, mUp) based on euler angles (mYaw, mPitch)
        void UpdateVectors();
    };
};
