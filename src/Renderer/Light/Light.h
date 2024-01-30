#pragma once

#include <glm/glm.hpp>

namespace Renderer
{
    class Light
    {
    public:
        Light();
        Light(const glm::vec3& colour, const float ambientIntensity, const float diffuseIntensity);
        ~Light() = default;

        glm::vec3 GetColour() const;
        float GetAmbientIntensity() const;
        float GetDiffuseIntensity() const;

    private:
        glm::vec3 mColour;
        float mAmbientIntensity;
        float mDiffuseIntensity;
    };
};
