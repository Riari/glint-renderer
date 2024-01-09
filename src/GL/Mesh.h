#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "Buffer/EBO.h"
#include "Buffer/VAO.h"
#include "Buffer/VBO.h"

namespace GL
{
    class Mesh
    {
    public:
        Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);

        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void SetScale(glm::vec3 scale);

        glm::mat4 GetModelMatrix() const;

        void Draw();

    private:
        std::vector<GLfloat> mVertices;
        std::vector<GLuint> mIndices;

        GL::VAO mVAO;
        GL::VBO mVBO;
        GL::EBO mEBO;

        glm::vec3 mPosition = glm::vec3(0.0f);
        glm::vec3 mRotation = glm::vec3(0.0f);
        glm::vec3 mScale = glm::vec3(1.0f);
    };
};
