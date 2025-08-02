#pragma once

#include "AssetEditor.h"
#include "../IEditor.h"

namespace DentyEngine
{
	class PrefabPreviewObject;
	class EventOnPrefabEditorOpened;
	class EventOnPrefabEditorClosed;

	class Prefab;

	class PrefabEditor final : public IEditor, public AssetEditor
	{
	public:
		PrefabEditor();
		~PrefabEditor() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(PrefabEditor)

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		void OnOpened(EventOnPrefabEditorOpened* e);
		void OnClosed(EventOnPrefabEditorClosed* e);

		[[nodiscard]]
		bool AssetExists() const override { return (_prefab != nullptr); }
	private:
		Ref<Prefab> _prefab;
		Ref<PrefabPreviewObject> _parent;

		// Holder
		std::vector<Ref<PrefabPreviewObject>> _children;
	};
}
