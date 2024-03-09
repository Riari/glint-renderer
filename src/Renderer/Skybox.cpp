#include "Skybox.h"

#include <glm/gtc/type_ptr.hpp>

#include "Asset/Manager.h"
#include "Asset/Type/Image.h"
#include "Asset/Type/Skybox.h"
#include "Util/GL.h"

#include "Constants.h"

namespace Renderer
{
    Skybox::Skybox(const std::string& name)
        : mShaderProgram(new GL::Shader::Program())
        , mMesh(new Mesh(PRIMITIVE_SKYBOX_VERTICES, PRIMITIVE_SKYBOX_INDICES, 8, { 3, 2, 3 }, false))
    {
        // TODO: Shader program shouldn't be initialised here - it should be fetched from a shader repository
        // TODO: For now, just assume the program builds successfully
        mShaderProgram->AddShader(GL_VERTEX_SHADER, Util::File::Read("shaders/skybox.vert.glsl"));
        mShaderProgram->AddShader(GL_FRAGMENT_SHADER, Util::File::Read("shaders/skybox.frag.glsl"));
        mShaderProgram->Link();

        const Asset::Type::Skybox& skybox = Asset::Manager::Get<Asset::Type::Skybox>(name);

        // TODO: This is horrendous :(
        std::vector<void*> textureData;
        textureData.resize(6);
        const Asset::Type::Image& right = Asset::Manager::Get<Asset::Type::Image>(skybox.mFaceMap.at(Asset::Type::SKYBOX_RIGHT));
        textureData[0] = right.data;
        const Asset::Type::Image& left = Asset::Manager::Get<Asset::Type::Image>(skybox.mFaceMap.at(Asset::Type::SKYBOX_LEFT));
        textureData[1] = left.data;
        const Asset::Type::Image& up = Asset::Manager::Get<Asset::Type::Image>(skybox.mFaceMap.at(Asset::Type::SKYBOX_UP));
        textureData[2] = up.data;
        const Asset::Type::Image& down = Asset::Manager::Get<Asset::Type::Image>(skybox.mFaceMap.at(Asset::Type::SKYBOX_DOWN));
        textureData[3] = down.data;
        const Asset::Type::Image& back = Asset::Manager::Get<Asset::Type::Image>(skybox.mFaceMap.at(Asset::Type::SKYBOX_BACK));
        textureData[4] = back.data;
        const Asset::Type::Image& front = Asset::Manager::Get<Asset::Type::Image>(skybox.mFaceMap.at(Asset::Type::SKYBOX_FRONT));
        textureData[5] = front.data;

        mTexture = new GL::Texture(GL_TEXTURE_CUBE_MAP, 10);
        mTexture->Generate(1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, false, textureData);
    }

    Skybox::~Skybox()
    {
        delete mShaderProgram;
        delete mMesh;
        delete mTexture;
    }

    void Skybox::Draw(const glm::mat4& projection, const glm::mat4& view) const
    {
        glDepthMask(GL_FALSE);

        mShaderProgram->Use();
        mShaderProgram->SetUniformMatrix4fv("view", glm::value_ptr(view));
        mShaderProgram->SetUniformMatrix4fv("projection", glm::value_ptr(projection));
        mShaderProgram->SetUniform1i("skybox", mTexture->GetUnit());
        mTexture->Bind();
        mMesh->Bind();
        mMesh->Draw();

        glDepthMask(GL_TRUE);
    }
}
