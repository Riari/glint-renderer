#pragma once

#include <nlohmann/json.hpp>

#include "Asset/Asset.h"

namespace Asset::Type
{
    struct Model : public Asset
    {
        unsigned char* data;

        ~Model() override
        {
            free(data);
        }
    };
};
