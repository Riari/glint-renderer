#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "Renderer/GL/Buffer/EBO.h"
#include "Renderer/GL/Buffer/VAO.h"
#include "Renderer/GL/Buffer/VBO.h"
#include "Material.h"

namespace Renderer
{
    class Model
    {
    public:
        Model(std::vector<GLfloat> vertices, std::vector<GLuint> indices, unsigned int vertexLength, std::vector<unsigned int> attributeSizes, bool generateNormals);
        ~Model();

        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void SetScale(glm::vec3 scale);

        /// @brief Generates normals based on the vertices and attribute information. CAUTION: overwrites existing normals!
        /// @param normalAttributeIndex The index into mAttributeSizes corresponding to normal size.
        void GenerateNormals(size_t normalAttributeIndex);

        glm::mat4 GetModelMatrix() const;

        void SetMaterial(Material* material);
        Material* GetMaterial() const;

        void Bind();
        void Draw();
        void Unbind();

    private:
        std::vector<GLfloat> mVertices;
        unsigned int mVertexLength;
        std::vector<unsigned int> mAttributeSizes;

        std::vector<GLuint> mIndices;

        Material* mMaterial{nullptr};

        GL::VAO mVAO;
        GL::VBO mVBO;
        GL::EBO mEBO;

        glm::vec3 mPosition = glm::vec3(0.0f);
        glm::vec3 mRotation = glm::vec3(0.0f);
        glm::vec3 mScale = glm::vec3(1.0f);
    };
};
