#pragma once

#include "System/Resource/ILibrary.h"
#include "System/Resource/Prefab.h"

namespace DentyEngine
{
	class EventOnPrefabOverrideButtonClick;

	class PrefabLibrary final : public ILibrary
	{
	public:
		PrefabLibrary();
		~PrefabLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(PrefabLibrary)

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(const Ref<Prefab>& prefab);

		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<Prefab>> Find(PrefabID id) const;

		[[nodiscard]]
		std::optional<Ref<Prefab>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;
	private:
		[[nodiscard]]
		bool IsPrefabFile(const String& path) const;

		//
		// Event functions.
		//
		void OnPrefabOverrideButtonClick(EventOnPrefabOverrideButtonClick* e);
	private:
		std::unordered_map<FilePath, Ref<Prefab>, FilePath::Hash> _caches;
	};
}
