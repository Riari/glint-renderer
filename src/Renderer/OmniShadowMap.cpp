#include "OmniShadowMap.h"

#include "Constants.h"

namespace Renderer
{
    OmniShadowMap::OmniShadowMap(int size, int textureUnit)
        : ShadowMap(size, GL_TEXTURE_CUBE_MAP, textureUnit)
    {
    }
}
