#pragma once

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <stb_image.h>

#include "Asset/Asset.h"

namespace Asset::Type
{
    struct Image : public Asset
    {
        int width;
        int height;
        int channels;
        unsigned char* data;

        ~Image() override
        {
            free(data);
        }
    };

    // TODO: Find a better way to define initialisers
    static bool InitialiseImage(const std::string& basePath, const std::string& binaryFilename, const std::string& assetFilename, nlohmann::json& outAssetJson)
    {
        if (!std::filesystem::exists(basePath + "/" + assetFilename))
        {
            outAssetJson["Type"] = "Image";
            outAssetJson["Name"] = binaryFilename.substr(0, binaryFilename.find_last_of("."));
            outAssetJson["Path"] = binaryFilename;

            return true;
        }

        return false;
    }

    // TODO: Find a better way to define loaders
    static void LoadImage(const std::string& basePath, const nlohmann::json& json, std::unordered_map<std::string, Asset*>& assets)
    {
        Image* image = new Image();

        std::string path = basePath + "/" + json.at("Path").get<std::string>();
        image->data = stbi_load(path.c_str(), &(image->width), &(image->height), &(image->channels), 0);

        // TODO: Implement asset validation
        if (!image->data) {
            spdlog::error("Failed to load image '{}' from path '{}'", json.at("Name").get<std::string>(), path);
            return;
        }

        assets.emplace(json.at("Name").get<std::string>(), std::move(image));
    }
};
