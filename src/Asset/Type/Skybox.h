#pragma once

#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <stb_image.h>

#include "Asset/Asset.h"

namespace Asset::Type
{
    inline const char* SKYBOX_RIGHT = "Right";
    inline const char* SKYBOX_LEFT = "Left";
    inline const char* SKYBOX_UP = "Up";
    inline const char* SKYBOX_DOWN = "Down";
    inline const char* SKYBOX_BACK = "Back";
    inline const char* SKYBOX_FRONT = "Front";

    typedef std::unordered_map<std::string, std::string> FaceMap;

    struct Skybox : public Asset
    {
        /// @brief A map of face identifiers to image asset names.
        FaceMap mFaceMap;
    };

    // TODO: Find a better way to define loaders
    static void LoadSkybox(const std::string& basePath, const nlohmann::json& json, std::unordered_map<std::string, Asset*>& assets)
    {
        Skybox* skybox = new Skybox();

        // TODO: This should validate that all of the faces are correctly specified and that the image assets are valid
        FaceMap faceMap = json.at("FaceMap").get<FaceMap>();
        for (const auto& [face, image] : faceMap)
        {
            skybox->mFaceMap.emplace(face, "Image::" + image);
        }

        assets.emplace(json.at("Name").get<std::string>(), std::move(skybox));
    }
};
