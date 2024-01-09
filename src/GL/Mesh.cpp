#include "Mesh.h"

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

namespace GL
{
    Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
        : mVertices(vertices)
        , mIndices(indices)
    {
        mVAO.Bind();

        mVBO.Bind();
        mVBO.Write(sizeof(vertices[0]) * vertices.size(), &mVertices[0], GL_STATIC_DRAW);

        mVAO.SetAttributePointer(0, 3, 3 * sizeof(float), (void*)0);
        mVAO.EnableAttributePointer(0);

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

    void Mesh::Draw()
    {
        assert(mVBO.IsBound() && mEBO.IsBound() && "Mesh buffers are not bound");

        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    }
};
