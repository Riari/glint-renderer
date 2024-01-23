#include "Mesh.h"

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

namespace World
{
    Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, unsigned int vertexLength, std::vector<unsigned int> attributeSizes)
        : mVertices(vertices)
        , mVertexLength(vertexLength)
        , mAttributeSizes(attributeSizes)
        , mIndices(indices)
    {
        // TODO: Remove this call when meshes are proper models imported with real normals
        // 2 is the assumed index into the attribute size for the normal attribute
        GenerateNormals(2);

        mVAO.Bind();

        mVBO.Bind();
        mVBO.Write(sizeof(vertices[0]) * vertices.size(), &mVertices[0], GL_STATIC_DRAW);

        unsigned int prevAttributesSize = 0;
        for (unsigned int i = 0; i < attributeSizes.size(); ++i)
        {
            mVAO.SetAttributePointer(i, attributeSizes[i], sizeof(vertices[0]) * vertexLength, (void*)(sizeof(vertices[0]) * prevAttributesSize));
            mVAO.EnableAttributePointer(i);
            prevAttributesSize += attributeSizes[i];
        }

        mEBO.Bind();
        mEBO.Write(sizeof(indices[0]) * indices.size(), &mIndices[0], GL_STATIC_DRAW);
    }

    void Mesh::SetPosition(glm::vec3 position)
    {
        mPosition = position;
    }

    void Mesh::SetRotation(glm::vec3 rotation)
    {
        mRotation = rotation;
    }

    void Mesh::SetScale(glm::vec3 scale)
    {
        mScale = scale;
    }

    void Mesh::GenerateNormals(size_t normalAttributeIndex)
    {
        unsigned int normalOffset{0};
        for (size_t i = 0; i < normalAttributeIndex; ++i)
        {
            normalOffset += mAttributeSizes[i];
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

    glm::mat4 Mesh::GetModelMatrix() const
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, mPosition);
        model = glm::rotate(model, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, mScale);

        return model;
    }

    void Mesh::SetMaterial(Material* material)
    {
        mMaterial = material;
    }

    Material* Mesh::GetMaterial() const
    {
        return mMaterial;
    }

    void Mesh::Draw()
    {
        assert(mVBO.IsBound() && mEBO.IsBound() && "Mesh buffers are not bound");

        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    }
};
