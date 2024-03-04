#include "OmniShadowMap.h"

#include "Constants.h"

namespace Renderer
{
    OmniShadowMap::OmniShadowMap(int size)
        : ShadowMap(size, GL_TEXTURE_CUBE_MAP, TEXTURE_UNIT_OMNIDIRECTIONAL_SHADOWMAP)
    {
    }
}
