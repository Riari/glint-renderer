#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Renderer/ShadowMap.h"

namespace Renderer
{
    class Light
    {
    public:
        Light();
        Light(const int shadowWidth, const int shadowHeight, const glm::vec3& colour, const float ambientIntensity, const float diffuseIntensity);
        ~Light();

        glm::vec3 GetColour() const;
        float GetAmbientIntensity() const;
        float GetDiffuseIntensity() const;

        ShadowMap* GetShadowMap() const;

    protected:
        // TODO: pass these values through
        glm::mat4 mLightProjection{glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f)};

    private:
        glm::vec3 mColour;
        float mAmbientIntensity;
        float mDiffuseIntensity;

        ShadowMap* mShadowMap;
    };
};
