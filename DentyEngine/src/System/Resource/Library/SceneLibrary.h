#pragma once

#include "System/Core/DString.h"
#include "System/Core/ID/SceneID.h"
#include "System/Resource/ILibrary.h"

namespace DentyEngine
{
	class Scene;

	class SceneLibrary final : public ILibrary
	{
	public:
		SceneLibrary();
		~SceneLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SceneLibrary)

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(Ref<Scene> scene);

		void Replace(const FilePath& filePath, Ref<Scene> scene);
		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<Scene>> Find(SceneID id) const;

		[[nodiscard]]
		std::optional<Ref<Scene>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		std::optional<Ref<Scene>> FindByName(std::string_view name) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;

		[[nodiscard]]
		bool IsSceneFile(const String& path) const;

		[[nodiscard]]
		bool Exists(const FilePath& filePath) const;

		[[nodiscard]]
		bool IsDuplicateID(SceneID id) const;
	private:
		std::unordered_map<FilePath, Ref<Scene>, FilePath::Hash> _caches;
	};
}
