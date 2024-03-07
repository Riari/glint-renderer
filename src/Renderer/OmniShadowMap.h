#pragma once

#include "ShadowMap.h"

namespace Renderer
{
    class OmniShadowMap : public ShadowMap
    {
    public:
        OmniShadowMap(int size, int textureUnit);
    };
}
