#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Renderer/ShadowMap.h"

namespace Renderer
{
    class Light
    {
    public:
        Light(ShadowMap* shadowMap);
        Light(
            glm::vec3 colour,
            float ambientIntensity,
            float diffuseIntensity,
            ShadowMap* shadowMap);
        ~Light();

        glm::vec3 GetColour() const;
        float GetAmbientIntensity() const;
        float GetDiffuseIntensity() const;

        ShadowMap* GetShadowMap() const;

    protected:
        glm::mat4 mProjection;

    private:
        glm::vec3 mColour;
        float mAmbientIntensity;
        float mDiffuseIntensity;

        ShadowMap* mShadowMap;
    };
};
