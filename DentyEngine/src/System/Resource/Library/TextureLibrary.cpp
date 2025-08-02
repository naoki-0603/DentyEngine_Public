#include "stdafx.h"

#include "TextureLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"
#include "Graphics/Renderer/CubeMap.h"

#include "Graphics/Renderer/Texture.h"
#include "Graphics/Renderer/Texture2D.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	// Supported image file types.
	const std::array<std::string, TextureLibrary::SUPPORTED_FILE_FORMAT_COUNT> TextureLibrary::SUPPORTED_FILE_FORMATS =
	{
		".png", ".jpg", ".bmp", ".JPG", ".JPEG", ".jpeg", ".dds"
	};

	TextureLibrary::TextureLibrary() :
		ILibrary(), _caches()
	{
	}

	void TextureLibrary::Initialize()
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		auto path = EditorContentBrowser::ENTRY_FILE_PATH;

		auto contentSerializer = [this](ID3D11Device* device, auto self, FilePath& folderPath) -> void
		{
			for (auto& content : std::filesystem::directory_iterator(folderPath.Get()))
			{
				const FilePath& entryPath = FilePath(content.path());
				std::string entry = entryPath.Filename().GetAsString();

				if (content.is_directory())
				{
					self(device, self, folderPath /= entry);
					folderPath = folderPath.ParentPath();
				}
				else
				{
					if (!IsSupportedFormat(entryPath))
						continue;

					FilePath entryPathJson = entryPath;
					entryPathJson.ReplaceExtension(TEXTURE_FILE_EXTENSION);

					// HACK
					const bool isCubeMap = (entryPath.Extension().Get() == ".dds");

					if (!entryPathJson.IsValid())
					{
						if (isCubeMap)
						{
							const auto cubeMap = std::make_shared<CubeMap>();
							cubeMap->Load(device, entryPath.GetAsString());
						}
						else
						{
							const auto texture = std::make_shared<Texture2D>();
							texture->Load(device, entryPath.GetAsString());
						}

						continue;
					}

					if (Exists(entryPath))
						continue;

					// Load from filepath.
					if (isCubeMap)
					{
						auto cubeMap = std::make_shared<CubeMap>();
						cubeMap->Load(entryPathJson.Get());

						Add(cubeMap);
					}
					else
					{
						auto texture = std::make_shared<Texture2D>();
						texture->Load(entryPathJson.Get());

						Add(texture);
					}
				}
			}
		};
		contentSerializer(device, contentSerializer, path);
	}

	void TextureLibrary::Update()
	{
	}

	void TextureLibrary::OnGui()
	{
	}

	void TextureLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void TextureLibrary::Finalize()
	{
		_caches.clear();
	}

	void TextureLibrary::OnSerialized() const
	{
	}

	void TextureLibrary::OnDeserialized()
	{
	}

	void TextureLibrary::Add(const Ref<Texture>& texture)
	{
		const auto& filePath = texture->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in TextureLibrary::Add)");
		}

		_caches.try_emplace(filePath, texture);
	}

	void TextureLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR

			DENTY_INFO_LOG << "Succeed to remove " << filePath << " texture from library!";

			_caches.erase(it);

			return;
		}

		DENTY_SET_ERR_COLOR

		DENTY_INFO_LOG << "Failed to remove " << filePath << " texture from library!";
	}

	template <>
	std::optional<Ref<Texture>> TextureLibrary::Find(const FilePath& filePath) const
	{
		for (const auto& texture : _caches)
		{
			if (!String::StrICmp(texture.first.GetAsString(), filePath.GetAsString()))
				continue;

			return std::static_pointer_cast<Texture>(texture.second);
		}

		return std::nullopt;
	}

	template <>
	std::optional<Ref<Texture2D>> TextureLibrary::Find(const FilePath& filePath) const
	{
		for (const auto& texture : _caches)
		{
			if (!String::StrICmp(texture.first.GetAsString(), filePath.GetAsString()))
				continue;

			return std::static_pointer_cast<Texture2D>(texture.second);
		}

		return std::nullopt;
	}

	template <>
	std::optional<Ref<CubeMap>> TextureLibrary::Find(const FilePath& filePath) const
	{
		for (const auto& texture : _caches)
		{
			if (!String::StrICmp(texture.first.GetAsString(), filePath.GetAsString()))
				continue;

			return std::static_pointer_cast<CubeMap>(texture.second);
		}

		return std::nullopt;
	}

	template <>
	std::optional<Ref<Texture2D>> TextureLibrary::Find(TextureID id) const
	{
		for (auto&& texture : _caches | std::views::values)
		{
			if (texture->GetTextureID() != id)
				continue;

			return std::static_pointer_cast<Texture2D>(texture);
		}

		return std::nullopt;
	}

	bool TextureLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool TextureLibrary::Exists(const FilePath& filePath) const
	{
		for (const auto& textureFilePath : _caches | std::ranges::views::keys)
		{
			if (textureFilePath != filePath)
				continue;

			return true;
		}

		return false;
	}

	bool TextureLibrary::IsTextureFile(const String& path) const
	{
		return (FilePath(path.CStr()).Extension() == TEXTURE_FILE_EXTENSION);
	}

	bool TextureLibrary::IsSupportedFormat(const FilePath& entryPath) const
	{
		// Check is supported format.
		for (int i = 0; i < SUPPORTED_FILE_FORMAT_COUNT; ++i)
		{
			const auto& format = SUPPORTED_FILE_FORMATS.at(i);

			// If supported format, return true.
			if (format == entryPath.Extension().GetAsString())
			{
				return true;
			}
		}

		// Not supported.
		return false;
	}

	//
	// Texture Library Common Paths
	//
	const FilePath TextureLibraryCommonPaths::TOOLBAR_PLAY_TEXTURE_FILE_PATH = "Assets\\Textures\\Editor\\Play.DentyTexture";
	const FilePath TextureLibraryCommonPaths::TOOLBAR_PAUSE_TEXTURE_FILE_PATH = "Assets\\Textures\\Editor\\Pause.DentyTexture";
	const FilePath TextureLibraryCommonPaths::TOOLBAR_STEP_TEXTURE_FILE_PATH = "Assets\\Textures\\Editor\\Step.DentyTexture";
	const FilePath TextureLibraryCommonPaths::TOOLBAR_RESUME_TEXTURE_FILE_PATH = "Assets\\Textures\\Editor\\Resume.DentyTexture";

	const FilePath TextureLibraryCommonPaths::DIRECTIONAL_LIGHT_GIZMO_FILE_PATH = "Assets\\Textures\\Editor\\Gizmos\\DirectionalLight.DentyTexture";
	const FilePath TextureLibraryCommonPaths::POINT_LIGHT_GIZMO_FILE_PATH = "Assets\\Textures\\Editor\\Gizmos\\PointLight.DentyTexture";
	const FilePath TextureLibraryCommonPaths::SPOT_LIGHT_GIZMO_FILE_PATH = "Assets\\Textures\\Editor\\Gizmos\\SpotLight.DentyTexture";
	const FilePath TextureLibraryCommonPaths::HEMISPHERE_LIGHT_GIZMO_FILE_PATH = "Assets\\Textures\\Editor\\Gizmos\\HemisphereLight.DentyTexture";
	const FilePath TextureLibraryCommonPaths::CAMERA_GIZMO_FILE_PATH = "Assets\\Textures\\Editor\\Gizmos\\Camera.DentyTexture";

	const FilePath TextureLibraryCommonPaths::CONTENT_BROWSER_FOLDER_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Folder.DentyTexture";
	const FilePath TextureLibraryCommonPaths::CONTENT_BROWSER_FOLDER_EMPTY_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\FolderEmpty.DentyTexture";
	const FilePath TextureLibraryCommonPaths::CONTENT_BROWSER_BACK_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Back.DentyTexture";

	const FilePath TextureLibraryCommonPaths::MATERIAL_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Material.DentyTexture";
	const FilePath TextureLibraryCommonPaths::CUBE_MATERIAL_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\CubeMaterial.DentyTexture";

	const FilePath TextureLibraryCommonPaths::COMMON_SCENE_FILE_PATH = "Assets\\Textures\\Editor\\Common\\Scene.DentyTexture";
	const FilePath TextureLibraryCommonPaths::COMMON_GAME_FILE_PATH = "Assets\\Textures\\Editor\\Common\\Game.DentyTexture";
	const FilePath TextureLibraryCommonPaths::COMMON_X_MARK_FILE_PATH = "Assets\\Textures\\Editor\\Common\\XMark.DentyTexture";

	const FilePath TextureLibraryCommonPaths::PREFAB_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Prefab.DentyTexture";
	const FilePath TextureLibraryCommonPaths::SKYBOX_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Skybox.DentyTexture";
	const FilePath TextureLibraryCommonPaths::AVATAR_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Avatar.DentyTexture";
	const FilePath TextureLibraryCommonPaths::MESH_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Mesh.DentyTexture";
	const FilePath TextureLibraryCommonPaths::COLLISION_MESH_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\CollisionMesh.DentyTexture";
	const FilePath TextureLibraryCommonPaths::APPROXIMATION_COLLISION_MESH_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\ApproximationCollisionMesh.DentyTexture";
	const FilePath TextureLibraryCommonPaths::SHADER_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Shader.DentyTexture";
	const FilePath TextureLibraryCommonPaths::SCENE_TREE_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\Reload.DentyTexture";
	const FilePath TextureLibraryCommonPaths::ANIMATION_CLIP_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\AnimationClip.DentyTexture";
	const FilePath TextureLibraryCommonPaths::ANIMATOR_CONTROLLER_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\AnimatorController.DentyTexture";
	const FilePath TextureLibraryCommonPaths::CSHARP_SCRIPT_DEFAULT_THUMBNAIL_FILE_PATH = "Assets\\Textures\\Editor\\ContentBrowser\\CSharpScript.DentyTexture";

	const FilePath TextureLibraryCommonPaths::ERROR_FILE_PTAH = "Assets\\Textures\\Core\\Error.DentyTexture";
	const FilePath TextureLibraryCommonPaths::TEXTURE_EMPTY_FILE_PATH = "Assets\\Textures\\Core\\TextureEmpty.DentyTexture";
}
