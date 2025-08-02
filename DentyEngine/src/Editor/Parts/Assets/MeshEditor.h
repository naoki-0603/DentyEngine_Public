#pragma once

#include "AssetEditor.h"

#include "Editor/Parts/IEditor.h"

namespace DentyEngine
{
	class Mesh;

	class EventOnMeshEditorOpened;
	class EventOnMeshEditorClosed;

	class MeshEditor final : public IEditor, public AssetEditor
	{
	public:
		MeshEditor();
		~MeshEditor() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(MeshEditor)

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		void OnOpened(EventOnMeshEditorOpened* e);
		void OnClosed(EventOnMeshEditorClosed* e);

		[[nodiscard]]
		bool AssetExists() const override { return (_mesh != nullptr); }
	private:
		Ref<Mesh> _mesh;

		std::string _previousScaleFactor;
		std::string _previousCoordinateSystem;
	};
}
