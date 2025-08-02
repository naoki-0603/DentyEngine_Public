#include "stdafx.h"

#include "ShaderLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "Graphics/Renderer/Shader.h"

#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
    const std::string ShaderLibrary::STATIC_MESH_SHADER_PATH = "Assets\\Shaders\\BuildIn\\StaticMesh.DentyShader";
    const std::string ShaderLibrary::SKINNED_MESH_SHADER_PATH = "Assets\\Shaders\\BuildIn\\SkinnedMesh.DentyShader";

    const std::string ShaderLibrary::STATIC_MESH_SHADOW_SHADER_PATH = "Assets\\Shaders\\BuildIn\\StaticMeshShadow.DentyShader";
    const std::string ShaderLibrary::SKINNED_MESH_SHADOW_SHADER_PATH = "Assets\\Shaders\\BuildIn\\SkinnedMeshShadow.DentyShader";

    const std::string ShaderLibrary::COLLISION_VISUALIZER_MESH_SHADER_PATH = "Assets\\Shaders\\BuildIn\\CollisionVisualizerMesh.DentyShader";

    const std::string ShaderLibrary::PHOTO_STATIC_MESH_SHADER_PATH = "Assets\\Shaders\\BuildIn\\PhotoStaticMesh.DentyShader";
    const std::string ShaderLibrary::PHOTO_SKINNED_MESH_SHADER_PATH = "Assets\\Shaders\\BuildIn\\PhotoSkinnedMesh.DentyShader";

    const std::string ShaderLibrary::MESH_TOON_SHADER_PATH = "Assets\\Shaders\\BuildIn\\MeshToon.DentyShader";
    const std::string ShaderLibrary::POST_EFFECT_MESH_SHADER_PATH = "Assets\\Shaders\\BuildIn\\PostEffectMesh.DentyShader";

    const std::string ShaderLibrary::DEBUG_DRAWER_PATH = "Assets\\Shaders\\BuildIn\\DebugDrawer.DentyShader";
    const std::string ShaderLibrary::CPU_PARTICLE_SHADER_PATH = "Assets\\Shaders\\BuildIn\\CPUParticle.DentyShader";
    const std::string ShaderLibrary::GRID_SHADER_PATH = "Assets\\Shaders\\BuildIn\\Grid.DentyShader";
    const std::string ShaderLibrary::LINE_SHADER_PATH = "Assets\\Shaders\\BuildIn\\Line.DentyShader";
    const std::string ShaderLibrary::EXPANDED_IMAGE_SHADER_PATH = "Assets\\Shaders\\BuildIn\\ExpandedImage.DentyShader";
    const std::string ShaderLibrary::SKYBOX_PATH = "Assets\\Shaders\\BuildIn\\Skybox.DentyShader";

    //
    // Post effects.
    //
    const std::string ShaderLibrary::FULL_SCREEN_QUAD_PATH = "Assets\\Shaders\\BuildIn\\FullScreenQuad.DentyShader";
    const std::string ShaderLibrary::GAUSSIAN_BLUR_HORIZONTAL_PATH = "Assets\\Shaders\\BuildIn\\GaussianBlurHorizontal.DentyShader";
    const std::string ShaderLibrary::GAUSSIAN_BLUR_VERTICAL_PATH = "Assets\\Shaders\\BuildIn\\GaussianBlurVertical.DentyShader";
    const std::string ShaderLibrary::BLOOM_PATH = "Assets\\Shaders\\BuildIn\\Bloom.DentyShader";

    //
    // Sprites.
    //
    const std::string ShaderLibrary::SPRITE_SHADER_PATH = "Assets\\Shaders\\BuildIn\\Sprite.DentyShader";
    const std::string ShaderLibrary::SPRITE_3D_SHADER_PATH = "Assets\\Shaders\\BuildIn\\Sprite3D.DentyShader";

    ShaderLibrary::ShaderLibrary() :
        _caches()
    {
    }

    void ShaderLibrary::Initialize()
    {
        auto shaderFolderPath = EditorContentBrowser::ENTRY_FILE_PATH;

        auto contentSerializer = [this](auto self, FilePath& folderPath) -> void
        {
            for (auto& content : std::filesystem::directory_iterator(folderPath.Get()))
            {
                const auto& entryPath = content.path();
                std::string entry = entryPath.filename().string();

                if (content.is_directory())
                {
                    self(self, folderPath /= entry);
                    folderPath = folderPath.ParentPath();
                }

                {
                    const String path = entryPath.string();
                    if (IsShaderFile(path))
                    {
                        const auto shader = Utils::CreateRef<Shader>();
                        shader->Load(entryPath);

                        Add(shader);
                    }
                }
                
            }
        };
        contentSerializer(contentSerializer, shaderFolderPath);
    }

    void ShaderLibrary::Update()
    {
    }

    void ShaderLibrary::OnGui()
    {
    }

    void ShaderLibrary::OnEvent([[maybe_unused]] Event* e)
    {
    }

    void ShaderLibrary::Finalize()
    {
    }

    void ShaderLibrary::OnSerialized() const
    {
    }

    void ShaderLibrary::OnDeserialized()
    {
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        const auto& filePath = shader->GetFilePathToSerialized();

        if (filePath.IsEmpty())
        {
            DENTY_ASSERT(false, "File path is empty! (in ShaderLibrary::Add)");
        }

        _caches.try_emplace(filePath, shader);
    }

    void ShaderLibrary::Remove(const FilePath& filePath)
    {
        auto&& it = _caches.find(filePath);

        if (it != _caches.end())
        {
            DENTY_SET_INFO_COLOR
            DENTY_INFO_LOG << "Succeed to remove " << filePath << " shader from library!";

            _caches.erase(it);

            return;
        }

        DENTY_SET_ERR_COLOR
        DENTY_INFO_LOG << "Failed to remove " << filePath << " shader from library!";
    }

    std::optional<Ref<Shader>> ShaderLibrary::Find(const FilePath& filePath) const
    {
        const auto& it = _caches.find(filePath);

        if (it != _caches.end())
        {
            return it->second;
        }

        return std::nullopt;
    }

    bool ShaderLibrary::Reload([[maybe_unused]] const FilePath& path)
    {
        DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

        return false;
    }

    bool ShaderLibrary::IsShaderFile(const String& path) const
    {
        return (path.Contains(SHADER_FILE_EXTENSION));
    }
};