#pragma once

#include <filesystem>
#include <functional>
#include <fstream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "Asset.h"
#include "Constants.h"
#include "Util/File.h"

namespace Asset
{
    /// @brief Initialisers initialise or update assets on disk. They may output JSON data to write to the asset's .asset file.
    /// @param [in] basePath The base path to the asset.
    /// @param [in] binaryFilename The name of the asset's binary file.
    /// @param [in] assetFilename The name of the asset's .asset file.
    /// @param [out] assetJson The JSON data for the asset (if successful).
    /// @return True if the asset was initialised successfully, false otherwise.
    using Initialiser = std::function<bool(const std::string& basePath, const std::string& binaryFilename, const std::string& assetFilename, nlohmann::json& outAssetJson)>;

    /// @brief Loaders are responsible for loading assets from disk.
    /// @param [in] basePath The base path to the asset.
    /// @param [in] assetJson The JSON data from the asset's .asset file.
    /// @param [in] assetMap The asset map to add the asset to.
    using Loader = std::function<void(const std::string& basePath, const nlohmann::json& assetJson, std::unordered_map<std::string, Asset*>& assetMap)>;

    class Manager
    {
    public:
        static void RegisterInitialiser(const std::string& fileExtension, Initialiser initialiser)
        {
            GetInstance().mInitialisers[fileExtension] = initialiser;
        }

        static void RegisterLoader(const std::string& typeName, Loader loader)
        {
            GetInstance().mLoaders[typeName] = loader;
        }

        static void Initialise()
        {
            auto& instance = GetInstance();

            for (const auto& entry : std::filesystem::recursive_directory_iterator(ASSETS_PATH)) {
                if (!entry.is_regular_file() || entry.path().extension() == ASSETS_EXTENSION) continue;

                std::string extension = entry.path().extension().string();

                auto it = instance.mInitialisers.find(extension);
                if (it != instance.mInitialisers.end())
                {
                    std::string basePath = entry.path().parent_path().string();
                    std::string binaryFilename = entry.path().filename().string();
                    std::string assetFilename = entry.path().filename().replace_extension(ASSETS_EXTENSION).string();
                    nlohmann::json json;

                    if (it->second(basePath, binaryFilename, assetFilename, json))
                    {
                        std::ofstream file(basePath + "/" + assetFilename);
                        file << json.dump(4);
                        file.flush();
                        file.close();

                        spdlog::info("[assets] Initialised '{}'", binaryFilename);
                    }
                }
                else
                {
                    spdlog::warn("[assets] No initialiser registered for file extension '{}'", extension);
                }
            }
        }

        static void LoadAssets()
        {
            auto& instance = GetInstance();

            for (const auto& entry : std::filesystem::recursive_directory_iterator(ASSETS_PATH)) {
                if (!entry.is_regular_file() || entry.path().extension() != ASSETS_EXTENSION) continue;

                std::string raw = Util::File::Read(entry.path().string());
                nlohmann::json json = nlohmann::json::parse(raw);
                std::string type = json.at("Type").get<std::string>();
                std::string name = json.at("Name").get<std::string>();

                // TODO: Temporary adjustment - this prefixes names with types to help avoid name collisions
                json.at("Name") = type + "::" + name;

                auto it = instance.mLoaders.find(type);
                if (it != instance.mLoaders.end())
                {
                    it->second(entry.path().parent_path().string(), json, instance.mAssets);
                    spdlog::info("[assets] Loaded '{}'", entry.path().filename().string());
                }
                else
                {
                    spdlog::error("[assets] No loader registered for asset type '{}'", type);
                }
            }
        }

        template<typename T>
        static const T& Get(const std::string& name)
        {
            static_assert(std::is_base_of<Asset, T>::value, "Asset type is not derived from Asset");

            auto& instance = GetInstance();

            auto it = instance.mAssets.find(name);
            if (it != instance.mAssets.end())
            {
                T* derivedAsset = dynamic_cast<T*>(it->second);

                if (derivedAsset)
                {
                    return *derivedAsset;
                }
                else
                {
                    spdlog::error("[assets] Failed to cast asset '{}' to type '{}'", name, typeid(T).name());
                }
            }
            else
            {
                spdlog::error("[assets] Asset '{}' not found", name);
            }

            return *reinterpret_cast<const T*>(nullptr);
        }

    private:
        Manager() {}
        ~Manager()
        {
            for (auto& asset : mAssets)
            {
                delete asset.second;
            }
            mAssets.clear();
        }

        static Manager& GetInstance()
        {
            static Manager instance;
            return instance;
        }

        std::unordered_map<std::string, Initialiser> mInitialisers;
        std::unordered_map<std::string, Loader> mLoaders;
        std::unordered_map<std::string, Asset*> mAssets;
    };
};
