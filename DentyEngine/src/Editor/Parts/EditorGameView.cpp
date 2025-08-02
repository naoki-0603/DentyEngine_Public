#include "stdafx.h"

#include "EditorGameView.h"

#include "System/Events/EventRender.h"
#include "System/Events/EventSystem.h"

#include "Graphics/Buffer/SceneBuffer.h"

#include "Graphics/Buffer/ColorBuffer.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
	EditorGameView::EditorGameView() :
		EditorView()
	{
	}

	void EditorGameView::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventRuntimeSceneChanged>(BIND_EVENT_FUNCTION(OnRuntimeSceneChanged));
		dispatcher.Dispatch<EventOnApplicationStateChangedToEdit>(BIND_EVENT_FUNCTION(OnApplicationStateChangedToEdit));
	}

	void EditorGameView::OnGui(Ref<Scene> activeScene)
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		EventRequiredGameViewColorData e = {};
		NOTIFY_EVENT(e);

		constexpr ImGuiWindowFlags windowFlags = {};

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, WINDOW_PADDING_Y));

		// HACK ŠÖ”‚É‚Ü‚Æ‚ß‚é
		static bool open = true;
		if (ImGui::Begin("GameView##Editor", &open, windowFlags))
		{
			EditorView::OnGui(activeScene);

			if (IsViewSizeChanged(_previousSize, _currentSize))
			{
				if (_currentSize.x >= 0.0f && _currentSize.y >= 0.0f)
				{
					// OnViewSizeChanged
					e.GetSceneBuffer()->OnResize(device, _currentSize);

					// Event
					EventOnGameViewSizeChanged onGameViewSizeChanged(_currentSize);
					NOTIFY_EVENT(onGameViewSizeChanged);
				}
			}

			const ImTextureID colorMap = *e.GetSceneBuffer()->GetColorBuffer()->GetResource();
			ImGui::Image(colorMap, ImVec2(_currentSize.x, _currentSize.y));

		}
		ImGui::End();

		ImGui::PopStyleVar(2);
	}

    void EditorGameView::OnRuntimeSceneChanged([[maybe_unused]] EventRuntimeSceneChanged* e)
    {
		EventRequiredGameViewColorData eventRequiredGameViewColorData = {};
		NOTIFY_EVENT(eventRequiredGameViewColorData);

		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
		eventRequiredGameViewColorData.GetSceneBuffer()->OnResize(
		    device, _currentSize
		);

		// Event
		EventOnGameViewSizeChanged onGameViewSizeChanged(_currentSize);
		NOTIFY_EVENT(onGameViewSizeChanged);
    }

    void EditorGameView::OnApplicationStateChangedToEdit([[maybe_unused]] EventOnApplicationStateChangedToEdit* e)
    {
		EventRequiredGameViewColorData eventRequiredGameViewColorData = {};
		NOTIFY_EVENT(eventRequiredGameViewColorData);

		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
		eventRequiredGameViewColorData.GetSceneBuffer()->OnResize(
		    device, _currentSize
		);

		// Event
		EventOnGameViewSizeChanged onGameViewSizeChanged(_currentSize);
		NOTIFY_EVENT(onGameViewSizeChanged);
    }
};