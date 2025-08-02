#pragma once

#include "System/Resource/ILibrary.h"

#include "Graphics/Mesh/SceneTree.h"

namespace DentyEngine
{
	namespace MeshResource
	{
		class SceneTree;
	}
	
	class SceneTreeLibrary final : public ILibrary
	{
	public:
		SceneTreeLibrary();
		~SceneTreeLibrary() override = default;

		void Initialize() override;
		void Finalize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		// Call on after _meshLibrary->Initialize function.
		void GenerateSceneTreesMeshes();

		void Add(Ref<MeshResource::SceneTree> sceneTree);

		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<MeshResource::SceneTree>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;
	private:
		[[nodiscard]]
		bool IsSceneTreeFile(const String&) const;

	private:
		using SceneTrees = std::unordered_map<FilePath, Ref<MeshResource::SceneTree>, FilePath::Hash>;

		SceneTrees _caches;
	};
}
