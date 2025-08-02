#pragma once

#include "System/Core/DString.h"
#include "../ILibrary.h"

#include "System/Core/ID/MeshID.h"

namespace DentyEngine
{
	class EventOnMaterialEditorClosed;
	class EventOnSceneSaved;
	class EventOnScenePlayButtonPressed;

	class Asset;
	class Mesh;
	class Material;

	class MeshLibrary final : public ILibrary
	{
	public:
		MeshLibrary();
		~MeshLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(MeshLibrary)

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(Ref<Mesh> mesh);

		void Remove(const FilePath& filePath);

		// Call on after SceneTreeLibrary::GenerateSceneTreesMeshes function.
		void AttachRootMeshes();

		template <class T>
		[[nodiscard]]
		std::optional<Ref<T>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;

		[[nodiscard]]
		bool Exists(const FilePath& filePath) const;

		//
		// Static variables.
		//
	public:
		//
		// BuildIn mesh paths.
		//
	    static const std::string BOX_FILE_PATH;
		static const std::string SPHERE_FILE_PATH;
		static const std::string CAPSULE_FILE_PATH;
		static const std::string PLANE_FILE_PATH;
	private:
		//
		// Event functions.
		//
		void OnMaterialEditorClosed(EventOnMaterialEditorClosed* e);

		void OnSceneSaved(EventOnSceneSaved* e);
		void OnScenePlayButtonPressed(EventOnScenePlayButtonPressed* e);
	private:
		[[nodiscard]]
		FilePath ConvertToStaticMeshPath(const std::filesystem::path&) const;

		[[nodiscard]]
		FilePath ConvertToSkinnedMeshPath(const std::filesystem::path&) const;

		[[nodiscard]]
		bool IsSupportedFormat(const std::filesystem::path&) const;

		[[nodiscard]]
		bool IsSerializedFileExists(const std::filesystem::path&, bool&) const;

		[[nodiscard]]
		bool IsSerializedFile(const std::filesystem::path&) const;
	private:
		std::unordered_map<FilePath, Ref<Mesh>, FilePath::Hash> _caches;
	private:
		static constexpr size_t SUPPORTED_FILE_FORMAT_COUNT = 22;
		static const std::array<std::string, SUPPORTED_FILE_FORMAT_COUNT> SUPPORTED_FILE_FORMATS;
	};
}
