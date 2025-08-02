#include "stdafx.h"

#include "EditorProfilerWindow.h"

#include "System/Profiler/MeasuredData.h"

#include "System/Events/EventEditor.h"

namespace DentyEngine
{
    EditorProfilerWindow::EditorProfilerWindow() :
        EditorWindow(), IEditor(), _textSize(16.0f)
    {
    }

    void EditorProfilerWindow::OnGui(Ref<Scene> activeScene)
    {
        // If not opened, return.
		if (not _open) return;

        const MeasuredData& measuredData = MeasuredData::GetInstance();

        const auto frameStates = measuredData.GetFrameStates();
        const auto functionStates = measuredData.GetFunctionStates();
        const auto renderingStates = measuredData.GetRenderingState();

		if (ImGui::Begin("EditorProfilerWindow##EditorWindow", &_open, IMGUI_DEFAULT_WINDOW_FLAGS))
		{
            const float originalFontSize = ImGui::GetFontSize();

            ImGui::GetFont()->FontSize = _textSize;

            // Main Frame state
            {
                //
                const auto& frameData = frameStates.at("Program::Run");

                ImGui::TextWrapped("FrameData (%s)", frameData.name.c_str());
                ImGui::TextWrapped("ElapsedTime(ms): %f", frameData.milliseconds);
                ImGui::TextWrapped("FPS: %d", static_cast<int32_t>(frameData.fps));

                ImGui::Spacing();

                // Render manager function data.
                {
                    const auto& renderManagerFunctionData = functionStates.at("RenderManager::OnSceneRender");

                    ImGui::TextWrapped("%s", renderManagerFunctionData.name.c_str());
                    ImGui::TextWrapped("ElapsedTime(ms): %f", renderManagerFunctionData.milliseconds);

                    if (ImGui::TreeNodeEx("Details##RenderManager_OnSceneRender"))
                    {
                        for (const auto& renderingState : renderingStates | std::views::values)
                        {
                            ImGui::TextWrapped("%s", renderingState.name.c_str());
                            ImGui::TextWrapped("ElapsedTime(ms): %f", renderingState.milliseconds);
                        }

                        ImGui::TreePop();
                    }
                }

                ImGui::Spacing();

                {
                    const bool isFunctionDataMultiple = (functionStates.size() > 1);

                    if (isFunctionDataMultiple)
                    {
                        for (const auto& functionState : functionStates)
                        {
                            if (functionState.first == "RenderManager::OnSceneRender")
                                continue;

                            ImGui::TextWrapped("%s", functionState.second.name.c_str());
                            ImGui::TextWrapped("ElapsedTime(ms): %f", functionState.second.milliseconds);
                        }
                    }
                }
                
            }

            ImGui::GetFont()->FontSize = originalFontSize;

            ImGui::Separator();

            ImGui::SliderFloat("TextSize##EditorProfilerWindow", &_textSize, 6.0f, 20.0f);

		}
        ImGui::End();

		// Create event.
		if (not _open)
		{
			EventOnProfilerWindowClosed eventOnProfilerWindowClosed;
			NOTIFY_EVENT(eventOnProfilerWindowClosed);
		}

    }

    void EditorProfilerWindow::OnEvent(Event* e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<EventOnProfilerWindowOpened>(BIND_EVENT_FUNCTION(OnOpened));
        dispatcher.Dispatch<EventOnProfilerWindowClosed>(BIND_EVENT_FUNCTION(OnClosed));
    }

    void EditorProfilerWindow::OnOpened([[maybe_unused]] Event* e)
    {
        _open = true;

        // Set window size.
		ImGui::SetNextWindowSize(ImVec2(
			IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y
		));
    }

    void EditorProfilerWindow::OnClosed([[maybe_unused]] Event* e)
    {
        _open = false;
    }
}
