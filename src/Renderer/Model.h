#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "Renderer/GL/Buffer/EBO.h"
#include "Renderer/GL/Buffer/VAO.h"
#include "Renderer/GL/Buffer/VBO.h"
#include "Material.h"
#include "Mesh.h"

namespace Renderer
{
    class Model
    {
    public:
        Model();
        ~Model();

        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void SetScale(glm::vec3 scale);

        glm::mat4 GetModelMatrix() const;

        void AddMesh(Mesh* mesh, Material* material);
        void AddMesh(Mesh* mesh, size_t materialIndex);
        void AddMaterial(Material* material);

        std::vector<Mesh*> GetMeshes() const;
        std::vector<Material*> GetMaterials() const;
        std::vector<size_t> GetMeshToMaterialIndices() const;

    private:
        std::vector<Mesh*> mMeshes;
        std::vector<Material*> mMaterials;
        std::vector<size_t> mMeshToMaterial;

        glm::vec3 mPosition = glm::vec3(0.0f);
        glm::vec3 mRotation = glm::vec3(0.0f);
        glm::vec3 mScale = glm::vec3(1.0f);
    };
};
