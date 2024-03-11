#pragma once

namespace Renderer
{
    class Context;
    class Scene;

    class IPass
    {
    public:
        virtual bool Init() const
        {
            return true;
        }

        virtual void Execute(const Context* context, const Scene* scene) = 0;
    };
};
