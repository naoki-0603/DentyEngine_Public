#pragma once

#include "../ILibrary.h"
#include "System/Core/ID/TextureID.h"

#include "System/Core/DString.h"

namespace DentyEngine
{
	class Event;

	class Texture;
	class Texture2D;

	class TextureLibrary final : public ILibrary
	{
	public:
		TextureLibrary();
		~TextureLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(TextureLibrary)

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(const Ref<Texture>& texture);

		void Remove(const FilePath& filePath);

		//
		// Getter
		//
		[[nodiscard]]
		const std::unordered_map<FilePath, Ref<Texture>, FilePath::Hash>& GetCaches() const { return _caches; }

		template <class T>
		[[nodiscard]]
		std::optional<Ref<T>> Find(const FilePath&) const;

		template <class T>
		[[nodiscard]]
		std::optional<Ref<T>> Find(TextureID id) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;

		[[nodiscard]]
		bool Exists(const FilePath&) const;
	private:
		[[nodiscard]]
		bool IsTextureFile(const String&) const;

		[[nodiscard]]
		bool IsSupportedFormat(const FilePath&) const;
	private:
		std::unordered_map<FilePath, Ref<Texture>, FilePath::Hash> _caches;
	private:
		static constexpr size_t SUPPORTED_FILE_FORMAT_COUNT = 7;
		static const std::array<std::string, SUPPORTED_FILE_FORMAT_COUNT> SUPPORTED_FILE_FORMATS;
	};

	struct TextureLibraryCommonPaths final 
	{
	public:
		TextureLibraryCommonPaths() noexcept = default;
		~TextureLibraryCommonPaths() noexcept = default;
	public:
		//
		// Tool bar images.
		//
		static const FilePath TOOLBAR_PLAY_TEXTURE_FILE_PATH;
		static const FilePath TOOLBAR_PAUSE_TEXTURE_FILE_PATH;
		static const FilePath TOOLBAR_STEP_TEXTURE_FILE_PATH;
		static const FilePath TOOLBAR_RESUME_TEXTURE_FILE_PATH;

		//
		// Gizmos
		//
		static const FilePath DIRECTIONAL_LIGHT_GIZMO_FILE_PATH;
		static const FilePath POINT_LIGHT_GIZMO_FILE_PATH;
		static const FilePath SPOT_LIGHT_GIZMO_FILE_PATH;
		static const FilePath HEMISPHERE_LIGHT_GIZMO_FILE_PATH;
		static const FilePath CAMERA_GIZMO_FILE_PATH;

		//
		// ContentBrowser images.
		//
		static const FilePath CONTENT_BROWSER_FOLDER_FILE_PATH;
		static const FilePath CONTENT_BROWSER_FOLDER_EMPTY_FILE_PATH;
		static const FilePath CONTENT_BROWSER_BACK_FILE_PATH;

		static const FilePath MATERIAL_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath CUBE_MATERIAL_DEFAULT_THUMBNAIL_FILE_PATH;

		//
		// Common
		//
		static const FilePath COMMON_SCENE_FILE_PATH;
		static const FilePath COMMON_GAME_FILE_PATH;
		static const FilePath COMMON_X_MARK_FILE_PATH;

		//
		// Asset default thumbnails.
		//
		static const FilePath PREFAB_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath SKYBOX_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath AVATAR_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath MESH_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath COLLISION_MESH_THUMBNAIL_FILE_PATH;
		static const FilePath APPROXIMATION_COLLISION_MESH_THUMBNAIL_FILE_PATH;
		static const FilePath SHADER_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath SCENE_TREE_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath ANIMATION_CLIP_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath ANIMATOR_CONTROLLER_DEFAULT_THUMBNAIL_FILE_PATH;
		static const FilePath CSHARP_SCRIPT_DEFAULT_THUMBNAIL_FILE_PATH;

		//
		// Core
		//
		static const FilePath ERROR_FILE_PTAH;
		static const FilePath TEXTURE_EMPTY_FILE_PATH;

	};
};