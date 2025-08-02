#pragma once

#include "IEditor.h"

namespace DentyEngine
{
	class EditorMenuBar final : public IEditor
	{
	public:
		EditorMenuBar();
		~EditorMenuBar() override = default;

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

	private:
		void FileMenuBar(Ref<Scene> activeScene);
		void CreateNewScene(Ref<Scene> activeScene);

		void LoadScene(Ref<Scene> activeScene);

		void WindowMenuBar(Ref<Scene> activeScene);
		void SettingsMenuBar(Ref<Scene> activeScene);

		void ProjectSettings(Ref<Scene> activeScene);

		// Save functions.
		void Save(Ref<Scene> activeScene);
		void SaveAs(Ref<Scene> activeScene);
		void CloneAs(Ref<Scene> activeScene);

		void Shortcuts(Ref<Scene> activeScene);
	};
};