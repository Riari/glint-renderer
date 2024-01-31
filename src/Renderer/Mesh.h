#pragma once

#include <vector>

#include <GL/glew.h>

#include "Renderer/GL/Buffer/EBO.h"
#include "Renderer/GL/Buffer/VAO.h"
#include "Renderer/GL/Buffer/VBO.h"

namespace Renderer
{
    class Mesh
    {
    public:
        Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, unsigned int vertexLength, std::vector<unsigned int> attributeSizes, bool generateNormals);
        ~Mesh();

        void Bind();
        void Draw();
        void Unbind();

    private:
        std::vector<GLfloat> mVertices;
        unsigned int mVertexLength;
        std::vector<unsigned int> mAttributeSizes;

        std::vector<GLuint> mIndices;

        GL::VAO mVAO;
        GL::VBO mVBO;
        GL::EBO mEBO;

        /// @brief Generates normals based on the vertices and attribute information. CAUTION: overwrites existing normals!
        /// @param normalAttributeIndex The index into mAttributeSizes corresponding to normal size.
        void GenerateNormals(size_t normalAttributeIndex);
    };
};
