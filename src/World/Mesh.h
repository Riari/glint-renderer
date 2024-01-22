#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "GL/Buffer/EBO.h"
#include "GL/Buffer/VAO.h"
#include "GL/Buffer/VBO.h"

namespace World
{
    class Mesh
    {
    public:
        Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, unsigned int vertexLength, std::vector<unsigned int> attributeSizes);

        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void SetScale(glm::vec3 scale);

        /// @brief Generates normals based on the vertices and attribute information. CAUTION: overwrites existing normals!
        /// @param normalAttributeIndex The index into mAttributeSizes corresponding to normal size.
        void GenerateNormals(size_t normalAttributeIndex);

        glm::mat4 GetModelMatrix() const;

        void Draw();

    private:
        std::vector<GLfloat> mVertices;
        unsigned int mVertexLength;
        std::vector<unsigned int> mAttributeSizes;

        std::vector<GLuint> mIndices;

        GL::VAO mVAO;
        GL::VBO mVBO;
        GL::EBO mEBO;

        glm::vec3 mPosition = glm::vec3(0.0f);
        glm::vec3 mRotation = glm::vec3(0.0f);
        glm::vec3 mScale = glm::vec3(1.0f);
    };
};
