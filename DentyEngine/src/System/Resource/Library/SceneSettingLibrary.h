#pragma once

#include "System/Core/DString.h"
#include "System/Resource/ILibrary.h"

namespace DentyEngine
{
	class SceneSetting;

	class SceneSettingLibrary final : public ILibrary
	{
	public:
		SceneSettingLibrary();
		~SceneSettingLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SceneSettingLibrary);

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(Ref<SceneSetting> sceneSetting);
		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<SceneSetting>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;

		[[nodiscard]]
		bool IsSceneSettingFile(const String& path) const;

		[[nodiscard]]
		bool Exists(const FilePath& filePath) const;
	private:
		std::unordered_map<FilePath, Ref<SceneSetting>, FilePath::Hash> _caches;
	};
}
