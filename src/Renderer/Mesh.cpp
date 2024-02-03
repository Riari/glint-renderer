#include "Mesh.h"

#include <glm/glm.hpp>

namespace Renderer
{
    Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, unsigned int vertexLength, std::vector<unsigned int> vertexLayout, bool generateNormals)
        : mVertices(vertices)
        , mVertexLength(vertexLength)
        , mVertexLayout(vertexLayout)
        , mIndices(indices)
    {
        if (generateNormals)
        {
            // TODO: Remove this call when all meshes are proper models imported with real normals.
            // 2 is the assumed index into the attribute size for the normal attribute
            GenerateNormals(2);
        }

        mVAO.Bind();

        mVBO.Bind();
        mVBO.Write(sizeof(vertices[0]) * vertices.size(), &mVertices[0], GL_STATIC_DRAW);

        unsigned int prevAttributesSize = 0;
        for (unsigned int i = 0; i < vertexLayout.size(); ++i)
        {
            mVAO.SetAttributePointer(i, vertexLayout[i], sizeof(vertices[0]) * vertexLength, (void*)(sizeof(vertices[0]) * prevAttributesSize));
            mVAO.EnableAttributePointer(i);
            prevAttributesSize += vertexLayout[i];
        }

        mEBO.Bind();
        mEBO.Write(sizeof(indices[0]) * indices.size(), &mIndices[0], GL_STATIC_DRAW);
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::Bind()
    {
        mVAO.Bind();
        mVBO.Bind();
        mEBO.Bind();
    }

    void Mesh::Draw()
    {
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    }

    void Mesh::Unbind()
    {
        mVAO.Unbind();
        mVBO.Unbind();
        mEBO.Unbind();
    }

    void Mesh::GenerateNormals(size_t normalAttributeIndex)
    {
        unsigned int normalOffset{0};
        for (size_t i = 0; i < normalAttributeIndex; ++i)
        {
            normalOffset += mVertexLayout[i];
        }

        for (size_t i = 0; i < mIndices.size(); i += 3)
        {
            unsigned int index0 = mIndices[i] * mVertexLength;
            unsigned int index1 = mIndices[i + 1] * mVertexLength;
            unsigned int index2 = mIndices[i + 2] * mVertexLength;

            glm::vec3 v1(mVertices[index1] - mVertices[index0], mVertices[index1 + 1] - mVertices[index0 + 1], mVertices[index1 + 2] - mVertices[index0 + 2]);
            glm::vec3 v2(mVertices[index2] - mVertices[index0], mVertices[index2 + 1] - mVertices[index0 + 1], mVertices[index2 + 2] - mVertices[index0 + 2]);

            glm::vec3 normal = glm::cross(v1, v2);
            normal = glm::normalize(normal);

            index0 += normalOffset;
            index1 += normalOffset;
            index2 += normalOffset;

            mVertices[index0] += normal.x;
            mVertices[index0 + 1] += normal.y;
            mVertices[index0 + 2] += normal.z;

            mVertices[index1] += normal.x;
            mVertices[index1 + 1] += normal.y;
            mVertices[index1 + 2] += normal.z;

            mVertices[index2] += normal.x;
            mVertices[index2 + 1] += normal.y;
            mVertices[index2 + 2] += normal.z;
        }

        for (size_t i = 0; i < mVertices.size() / mVertexLength; ++i)
        {
            unsigned int normalIndex = i * mVertexLength + normalOffset;
            glm::vec3 normal(mVertices[normalIndex], mVertices[normalIndex + 1], mVertices[normalIndex + 2]);
            normal = glm::normalize(normal);
            mVertices[normalIndex] = normal.x;
            mVertices[normalIndex + 1] = normal.y;
            mVertices[normalIndex + 2] = normal.z;
        }
    }
};