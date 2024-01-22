#include "Camera.h"

namespace World
{
    Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float moveSpeed, float turnSpeed)
        : mPosition(position)
        , mWorldUp(worldUp)
        , mFront(glm::vec3(0.0f, 0.0f, -1.0f))
        , mYaw(yaw)
        , mPitch(pitch)
        , mMoveSpeed(moveSpeed)
        , mTurnSpeed(turnSpeed)
    {
        UpdateVectors();
    }

    Camera::~Camera()
    {

    }

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(mPosition, mPosition + mFront, mUp);
    }

    void Camera::Update(App::Window* pWindow, float deltaTime)
    {
        float velocity = mMoveSpeed * static_cast<float>(deltaTime);

        if (pWindow->IsKeyPressed(GLFW_KEY_W))
        {
            mPosition += mFront * velocity;
        }

        if (pWindow->IsKeyPressed(GLFW_KEY_S))
        {
            mPosition -= mFront * velocity;
        }

        if (pWindow->IsKeyPressed(GLFW_KEY_A))
        {
            mPosition -= mRight * velocity;
        }

        if (pWindow->IsKeyPressed(GLFW_KEY_D))
        {
            mPosition += mRight * velocity;
        }

        float mouseDeltaX = pWindow->GetMouseDeltaX();
        float mouseDeltaY = pWindow->GetMouseDeltaY();

        if (mouseDeltaX == 0.0f && mouseDeltaY == 0.0f)
        {
            return;
        }

        mYaw += mTurnSpeed * mouseDeltaX;
        mPitch += mTurnSpeed * mouseDeltaY;
        mPitch = glm::clamp(mPitch, -89.0f, 89.0f);

        UpdateVectors();
    }

    void Camera::UpdateVectors()
    {
        mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront.y = sin(glm::radians(mPitch));
        mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront = glm::normalize(mFront);

        mRight = glm::normalize(glm::cross(mFront, mWorldUp));
        mUp = glm::normalize(glm::cross(mRight, mFront));
    }
};
