#pragma once

#include <GL/glew.h>

#include "Map.h"

namespace Renderer
{
    class Material
    {
    public:
        Material();
        Material(Map* baseMap, GLfloat specularIntensity, GLfloat shininess);
        ~Material();

        Map* GetBaseMap() const;
        GLfloat GetSpecularIntensity() const;
        GLfloat GetShininess() const;

        void Bind() const;
        void Unbind() const;

    private:
        Map* mBaseMap;

        GLfloat mSpecularIntensity;
        GLfloat mShininess;
    };
};
