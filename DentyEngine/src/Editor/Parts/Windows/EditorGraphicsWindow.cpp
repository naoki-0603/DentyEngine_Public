#include "stdafx.h"

#include "EditorGraphicsWindow.h"

#include "System/Events/EventEditor.h"

namespace DentyEngine
{
	EditorGraphicsWindow::EditorGraphicsWindow() :
		EditorWindow(), IEditor()
	{
	}

	void EditorGraphicsWindow::OnGui(Ref<Scene> activeScene)
	{
		// If not opened, return.
		if (not _open) return;

		if (ImGui::Begin("EditorGraphicsWindow##EditorWindow", &_open, IMGUI_DEFAULT_WINDOW_FLAGS))
		{

		}
		ImGui::End();

		// Create event.
		if (not _open)
		{
			EventOnGraphicsWindowClosed eventOnGraphicsWindowClosed;
			NOTIFY_EVENT(eventOnGraphicsWindowClosed);
		}

	}

	void EditorGraphicsWindow::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnGraphicsWindowOpened>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventOnGraphicsWindowClosed>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void EditorGraphicsWindow::OnOpened([[maybe_unused]] Event* e)
	{
		_open = true;

		// Set window size.
		ImGui::SetNextWindowSize(ImVec2(
			IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y
		));
	}

	void EditorGraphicsWindow::OnClosed([[maybe_unused]] Event* e)
	{
		_open = false;
	}
}
