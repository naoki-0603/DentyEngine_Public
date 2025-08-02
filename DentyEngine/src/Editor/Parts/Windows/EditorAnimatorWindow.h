#pragma once

#include "Editor/Parts/IEditor.h"

#include "EditorWindow.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
	class AnimatorController;

	class EventOnAnimatorWindowOpened;
	class EventOnAnimatorWindowClosed;

	class EditorAnimatorWindow final : public EditorWindow, public IEditor
	{
	public:
		EditorAnimatorWindow();
		~EditorAnimatorWindow() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(EditorAnimatorWindow)

		void OnGui(Ref<Scene> activeScene) override;
		void OnEvent(Event* e) override;

		void OnOpened(Event* e) override;
		void OnClosed(Event* e) override;
	private:
		//
		// Drag and drop receive functions.
		//
		void AnimationClipAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData);

		void ParameterWindow();
		void SequenceWindow();
		void InspectorWindow();

		void Restore();

		[[nodiscard]]
		bool HasController() const { return (_controller != nullptr); }
	private:
		Ref<AnimatorController> _controller;

		bool _addParameterButtonPressed;
	};
}
