#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace App { class Window; }

namespace Renderer
{
    namespace GL::Shader { class Program; }
    class Model;

    class Context
    {
    public:
        Context(App::Window& window);
        ~Context() = default;

        const App::Window& GetWindow() const;
        glm::mat4 GetProjectionMatrix() const;

        void DrawModel(GL::Shader::Program* shaderProgram, const Model* model, bool withMaterials) const;
        void DrawModels(GL::Shader::Program* shaderProgram, std::vector<Model*> models, bool withMaterials) const;

    private:
        const App::Window& mWindow;
    };
};
