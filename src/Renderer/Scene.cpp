#include "Scene.h"

#include <GL/glew.h>

#include "Camera.h"
#include "Model.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Skybox.h"

namespace Renderer
{
    Scene::~Scene()
    {
        delete mCamera;

        for (auto& model : mModels)
        {
            delete model;
        }
        mModels.clear();

        delete mDirectionalLight;

        for (auto& light : mPointLights)
        {
            delete light;
        }
        mPointLights.clear();

        for (auto& light : mSpotLights)
        {
            delete light;
        }
        mSpotLights.clear();

        delete mSkybox;
    }

    void Scene::SetCamera(Camera* camera)
    {
        mCamera = camera;
    }

    void Scene::AddModel(Model* model)
    {
        mModels.push_back(model);
    }

    void Scene::AddModels(std::vector<Model*> models)
    {
        mModels.insert(mModels.end(), models.begin(), models.end());
    }

    void Scene::SetDirectionalLight(DirectionalLight* light)
    {
        mDirectionalLight = light;
    }

    void Scene::AddPointLight(PointLight* light)
    {
        mPointLights.push_back(light);
    }

    void Scene::AddSpotLight(SpotLight* light)
    {
        mSpotLights.push_back(light);
    }

    void Scene::SetSkybox(Skybox* skybox)
    {
        mSkybox = skybox;
    }

    Camera* Scene::GetCamera() const
    {
        return mCamera;
    }

    std::vector<Model*> Scene::GetModels() const
    {
        return mModels;
    }

    DirectionalLight* Scene::GetDirectionalLight() const
    {
        return mDirectionalLight;
    }

    std::vector<PointLight*> Scene::GetPointLights() const
    {
        return mPointLights;
    }

    std::vector<SpotLight*> Scene::GetSpotLights() const
    {
        return mSpotLights;
    }

    Skybox* Scene::GetSkybox() const
    {
        return mSkybox;
    }
};
