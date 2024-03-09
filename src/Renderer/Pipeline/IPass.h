#pragma once

namespace Renderer
{
    class IPass
    {
    public:
        virtual ~IPass() = 0;

        // TODO: This should probably take in a renderer context so it can access things like camera matrices
        virtual void Execute() = 0;
    };
};
