#include "Model.h"

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer
{
    Model::Model()
    {
    }

    Model::~Model()
    {
    }

    void Model::SetPosition(glm::vec3 position)
    {
        mPosition = position;
    }

    void Model::SetRotation(glm::vec3 rotation)
    {
        mRotation = rotation;
    }

    void Model::SetScale(glm::vec3 scale)
    {
        mScale = scale;
    }

    glm::mat4 Model::GetModelMatrix() const
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, mPosition);
        model = glm::rotate(model, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, mScale);

        return model;
    }

    void Model::AddMesh(Mesh* mesh, Material* material)
    {
        mMaterials.push_back(material);
        size_t materialIndex = mMaterials.size() - 1;
        AddMesh(mesh, materialIndex);
    }

    void Model::AddMesh(Mesh* mesh, size_t materialIndex)
    {
        mMeshes.push_back(mesh);
        mMeshToMaterial.push_back(materialIndex);
    }

    void Model::AddMaterial(Material* material)
    {
        mMaterials.push_back(material);
    }

    std::vector<Mesh*> Model::GetMeshes() const
    {
        return mMeshes;
    }

    std::vector<Material*> Model::GetMaterials() const
    {
        return mMaterials;
    }

    std::vector<size_t> Model::GetMeshToMaterialIndices() const
    {
        return mMeshToMaterial;
    }
};
