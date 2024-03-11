#pragma once

#include <vector>

namespace Renderer
{
    class Camera;
    class Model;
    class DirectionalLight;
    class PointLight;
    class SpotLight;
    class Skybox;

    class Scene
    {
    public:
        Scene() = default;
        ~Scene();

        void SetCamera(Camera* camera);
        void AddModel(Model* model);
        void AddModels(std::vector<Model*> models);
        void SetDirectionalLight(DirectionalLight* light);
        void AddPointLight(PointLight* light);
        void AddSpotLight(SpotLight* light);
        void SetSkybox(Skybox* skybox);

        Camera* GetCamera() const;
        std::vector<Model*> GetModels() const;
        DirectionalLight* GetDirectionalLight() const;
        std::vector<PointLight*> GetPointLights() const;
        std::vector<SpotLight*> GetSpotLights() const;
        Skybox* GetSkybox() const;

    private:
        Camera* mCamera;
        std::vector<Model*> mModels;
        DirectionalLight* mDirectionalLight;
        std::vector<PointLight*> mPointLights;
        std::vector<SpotLight*> mSpotLights;
        Skybox* mSkybox;
    };
};
