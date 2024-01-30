#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "Asset/Type/Image.h"
#include "Asset.h"
#include "Util/File.h"

namespace Asset
{
    using Loader = std::function<void(const std::string& basePath, const nlohmann::json&, std::unordered_map<std::string, Asset*>&)>;

    const char* ASSETS_PATH = "assets/";
    const char* ASSETS_EXTENSION = ".asset";

    class Manager
    {
    public:
        static void RegisterType(const std::string& typeName, Loader loader)
        {
            GetInstance().mLoaders[typeName] = loader;
        }

        static void LoadAssets()
        {
            auto& instance = GetInstance();

            for (const auto& entry : std::filesystem::recursive_directory_iterator(ASSETS_PATH)) {
                if (!entry.is_regular_file() || entry.path().extension() != ASSETS_EXTENSION) continue;

                std::string raw = Util::File::Read(entry.path().string());
                nlohmann::json json = nlohmann::json::parse(raw);
                std::string type = json.at("Type").get<std::string>();

                auto it = instance.mLoaders.find(type);
                if (it != instance.mLoaders.end()) {
                    it->second(entry.path().parent_path().string(), json, instance.mAssets);
                    spdlog::info("[assets] Loaded '{}'", entry.path().filename().string());
                } else {
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
            if (it != instance.mAssets.end()) {
                T* derivedAsset = dynamic_cast<T*>(it->second);

                if (derivedAsset) {
                    return *derivedAsset;
                } else {
                    spdlog::error("[assets] Failed to cast asset '{}' to type '{}'", name, typeid(T).name());
                }
            } else {
                spdlog::error("[assets] Asset '{}' not found", name);
            }

            return *reinterpret_cast<const T*>(nullptr);
        }

    private:
        Manager() {}
        ~Manager()
        {
            for (auto& asset : mAssets) {
                delete asset.second;
            }
            mAssets.clear();
        }

        static Manager& GetInstance()
        {
            static Manager instance;
            return instance;
        }

        std::unordered_map<std::string, Loader> mLoaders;
        std::unordered_map<std::string, Asset*> mAssets;
    };
};
