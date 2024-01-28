#pragma once

#include <nlohmann/json.hpp>

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
};
