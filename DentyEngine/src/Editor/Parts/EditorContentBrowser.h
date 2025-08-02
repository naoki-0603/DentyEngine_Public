#pragma once

#include "IEditor.h"

#include "System/Resource/Asset.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

#include "System/Core/DString.h"

namespace DentyEngine
{
	namespace Utils
	{
		// Set has asset original format or not to second parameter.
		[[nodiscard]]
		static Asset::AssetType ObtainAssetTypeFromFilePath(const FilePath&, bool&);
	}

	class Texture2D;

	class ColorBuffer;

	struct EditorContentBrowserTextures
	{
		EditorContentBrowserTextures();

		Ref<Texture2D> folderEmpty;
		Ref<Texture2D> folderFileExists;
		Ref<Texture2D> backArrow;
	};

	enum class EditorContentBrowserExistsFileType : int32_t
	{
		AnimationClip = BIT(0),
		AnimatorController = BIT(1),
		Texture = BIT(2),
		Scene = BIT(3),
		Mesh = BIT(4),
		ApproximationCollisionMesh = BIT(5),
		CollisionMesh = BIT(6),
		Material = BIT(7),
		Prefab = BIT(8),
		Shader = BIT(9),
		Avatar = BIT(10),
		MeshSceneTree = BIT(11),
		Skybox = BIT(12),
		Script = BIT(13),
		CubeMaterial = BIT(14),

		Max = 15
	};

	class EditorContentBrowser final : public IEditor
	{
	public:
		EditorContentBrowser(ID3D11Device* device);
		~EditorContentBrowser() noexcept override;

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		//
		// Static functions.
		//
		[[nodiscard]]
		static std::optional<std::string> MakeUniqueNameInDirectory(const FilePath& directory, std::string_view name, Asset::AssetType assetType, const uint32_t maxTrialCount = 100);

		[[nodiscard]]
		static bool IsNameExists(const FilePath& directory, std::string_view name, Asset::AssetType assetType);

		// If duplicate return true.
		[[nodiscard]]
		static bool NameAlreadyExists(std::string_view name, std::string_view directoryFilePath, std::string_view targetExtension);

		// If can't make unique name, return empty string.
		[[nodiscard]]
		static std::optional<std::string> MakeUniqueName(std::string_view name, std::string_view directoryFilePath, std::string_view targetExtension, const int numberOfTrials = 100);
	public:
		// This file path should be changed for each project.
		static const FilePath ENTRY_FILE_PATH;

		// others.
		static const Vector2 BACK_ARROW_IMAGE_SIZE;
		static const Vector2 THUMBNAIL_SIZE;
		static const Vector2 PADDING;
		static const Vector2 CEIL_SIZE;

		static const Vector2 DOWN_SAMPLING_THUMBNAIL_SIZE;

	private:
		// Folders.
		void SceneAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void MaterialAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void CubeMaterialAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void MeshAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void CollisionMeshAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void ApproximationCollisionMeshAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void MeshSceneTreeAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void AvatarAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void AnimationAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void AnimatorControllerAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void TextureAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void PrefabAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void ShaderAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void SkyboxAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);
		void ScriptAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay);

		// Update file.
		void UpdateSceneAssets();
		void UpdateMaterialAssets();
		void UpdateMeshAssets();
		void UpdateAnimationAssets();
		void UpdateTextureAssets();
		void UpdatePrefabAssets();
		void UpdateShaderAssets();

		void CreateAssetOnContentBrowser();
		void ModifyAssetOnContentBrowser();

		void SceneChangeModalWindow(std::string_view filePath);

		//
		// Drag and drop source callback functions.
		//
		void SceneAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void MeshAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void CollisionMeshAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void ApproximationCollisionMeshAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void MaterialAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void AnimationClipAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void AnimationControllerAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void TextureAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void PrefabAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void SkyboxAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
		void ScriptAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);

		//
		// Drag and drop target callback functions.
		//
		void PrefabAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);

		//
		// Reload functions.
		//
		void Reload();
		void ReloadAll();

		[[nodiscard]]
		String ObtainDisplayText(const String&, bool) const;

		[[nodiscard]]
		EditorContentBrowserExistsFileType ObtainExistsFileType(const String&, bool&) const;

		[[nodiscard]]
		bool IsAnimationClip(const String&) const;

		[[nodiscard]]
		bool IsAnimatorController(const String&) const;

		[[nodiscard]]
		bool IsTexture(const String&) const;

		[[nodiscard]]
		bool IsScene(const String&) const;

		[[nodiscard]]
		bool IsMesh(const String&) const;

		[[nodiscard]]
		bool IsCollisionMesh(const String&) const;

		[[nodiscard]]
		bool IsApproximationCollisionMesh(const String&) const;

		[[nodiscard]]
		bool IsMeshSceneTree(const String&) const;

		[[nodiscard]]
		bool IsAvatar(const String&) const;

		[[nodiscard]]
		bool IsMaterial(const String&) const;

		[[nodiscard]]
		bool IsPrefab(const String&) const;

		[[nodiscard]]
		bool IsShader(const String&) const;

		[[nodiscard]]
		bool IsSkybox(const String&) const;

		[[nodiscard]]
		bool IsScript(const String&) const;

		//
		// Event functions.
		//
		void OnDirectoryChanged();
	private:
		std::unordered_map<std::string, Ref<ColorBuffer>> _previewPhotos;
		
		// Textures.
		EditorContentBrowserTextures _editorContentBrowserTextures;
		
		// current control asset.
		/*AssetControl _currentControlAsset;*/

		FilePath _currentDirectory;
		FilePath _previousDirectory;

		std::string _clickedSceneName;

		bool _isSceneClicked;
	};

	DEFINE_ENUM_CLASS_BIT_OPERATORS(EditorContentBrowserExistsFileType)
};