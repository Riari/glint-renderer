#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "Asset/Asset.h"

namespace Asset::Type
{
    struct MeshData
    {
        std::vector<float> mVertices;
        std::vector<unsigned int> mIndices;
        unsigned int mVertexLength;
        std::vector<unsigned int> mAttributeSizes;
        unsigned int mMaterialIndex;
    };

    struct MaterialData
    {
        bool mHasTexture;
        std::string mTextureAssetName;
        float mSpecularIntensity;
        float mShininess;
    };

    struct Model : public Asset
    {
        std::vector<MeshData*> mMeshes;
        std::vector<MaterialData*> mMaterials;

        ~Model() override
        {
            for (size_t i = 0; i < mMeshes.size(); ++i)
            {
                delete mMeshes[i];
            }
            mMeshes.clear();

            for (size_t i = 0; i < mMaterials.size(); ++i)
            {
                delete mMaterials[i];
            }
            mMaterials.clear();
        }
    };

    // TODO: Find a better way to define loaders
    // TODO: Instead of treating model assets as self-contained mesh/material/texture packages,
    //       they should be broken down into distinct asset types with appropriate references.
    //       Textures should be loaded as Image assets, materials should be loaded as
    //       Material assets, and mesh data should be loaded as MeshScene assets.
    //       Model assets would then simply contain a list of MeshScene references and Material
    //       references, where Material assets further reference Image assets for their textures.
    class ModelLoader
    {
    public:
        static void Load(const std::string& basePath, const nlohmann::json& json, std::unordered_map<std::string, Asset*>& assets)
        {
            Model* model = new Model();

            Assimp::Importer importer;
            const std::string path = basePath + "/" + json.at("Path").get<std::string>();

            // TODO: Where possible, these flags should probably be set dynamically based on the format being imported.
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_MakeLeftHanded | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

            if (!scene)
            {
                spdlog::error("Model ({}) failed to load: {}", path, importer.GetErrorString());
                return;
            }

            model->mMeshes = LoadNode(scene->mRootNode, scene);
            model->mMaterials = LoadMaterials(scene);

            assets.emplace(json.at("Name").get<std::string>(), std::move(model));
        }

        static std::vector<MeshData*> LoadNode(aiNode* node, const aiScene* scene)
        {
            std::vector<MeshData*> meshes;

            for (size_t i = 0; i < node->mNumMeshes; ++i)
            {
                meshes.push_back(LoadMesh(scene->mMeshes[node->mMeshes[i]], scene));
            }

            for (size_t i = 0; i < node->mNumChildren; ++i)
            {
                std::vector<MeshData*> nodeMeshes = LoadNode(node->mChildren[i], scene);
                meshes.insert(meshes.end(), nodeMeshes.begin(), nodeMeshes.end());
            }

            return meshes;
        }

        static MeshData* LoadMesh(aiMesh* mesh, const aiScene* scene)
        {
            MeshData* data = new MeshData;

            // TODO: For now, these are hardcoded
            data->mVertexLength = 8;
            data->mAttributeSizes = { 3, 2, 3 };

            for (size_t i = 0; i < mesh->mNumVertices; i++)
            {
                data->mVertices.insert(data->mVertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
                if (mesh->mTextureCoords[0])
                {
                    data->mVertices.insert(data->mVertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
                }
                else
                {
                    data->mVertices.insert(data->mVertices.end(), { 0.0f, 0.0f });
                }
                data->mVertices.insert(data->mVertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
            }

            for (size_t i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (size_t j = 0; j < face.mNumIndices; j++)
                {
                    data->mIndices.push_back(face.mIndices[j]);
                }
            }

            data->mMaterialIndex = mesh->mMaterialIndex;

            return data;
        }

        static std::vector<MaterialData*> LoadMaterials(const aiScene* scene)
        {
            std::vector<MaterialData*> materials(scene->mNumMaterials);

            // TODO: For now, these properties are hardcoded
            float defaultSpecularIntensity = 2.0f;
            float defaultShininess = 64.0f;

            for (size_t i = 0; i < scene->mNumMaterials; i++)
            {
                aiMaterial* material = scene->mMaterials[i];

                materials[i] = nullptr;

                if (material->GetTextureCount(aiTextureType_DIFFUSE))
                {
                    aiString path;
                    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
                    {
                        int idx = std::string(path.data).rfind("\\");

                        // TODO: Replace this workaround for referencing the image assets to use as textures
                        // when there's a proper pipeline for materials as distinct asset types.
                        std::string imageAssetName = "Image::" + std::string(path.data).substr(idx + 1);
                        imageAssetName = imageAssetName.substr(0, imageAssetName.find_last_of("."));
                        materials[i] = new MaterialData{true, imageAssetName, defaultSpecularIntensity, defaultShininess};
                    }
                }
                else
                {
                    materials[i] = new MaterialData{false, "", defaultSpecularIntensity, defaultShininess};
                }
            }

            return materials;
        }
    };
};
