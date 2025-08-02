#include "stdafx.h"

#include "EditorAnimatorWindow.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Events/EventEditor.h"

#include "System/Resource/Animations/AnimatorController.h"

namespace DentyEngine
{
	EditorAnimatorWindow::EditorAnimatorWindow() :
		EditorWindow(), IEditor(), _controller(), _addParameterButtonPressed()
	{

	}

	void EditorAnimatorWindow::OnGui(Ref<Scene> activeScene)
	{
		if (not _open) return;

		if (HasController())
		{
			_controller->Update();
		}

		if (ImGui::Begin("Animator##AnimatorWindow", &_open))
		{
			if (HasController())
			{
				ParameterWindow();

				ImGui::SameLine();

				SequenceWindow();

				ImGui::SameLine();

				InspectorWindow();
			}

		}
		ImGui::End();


		// If closed.
		if (not _open)
		{
			if (_controller)
			{
				_controller->Save(true);
			}

			EventOnAnimatorWindowClosed eventOnAnimatorWindowClosed;
			NOTIFY_EVENT(eventOnAnimatorWindowClosed);
		}
	}

	void EditorAnimatorWindow::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnAnimatorWindowOpened>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventOnAnimatorWindowClosed>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void EditorAnimatorWindow::OnOpened(Event* e)
	{
		// If already opened
		if (_open)
		{
			// Notify closed event.
			EventOnAnimatorWindowClosed eventOnAnimatorWindowClosed;
			NOTIFY_EVENT(eventOnAnimatorWindowClosed);
		}

		_open = true;

		// Set window size.
		ImGui::SetNextWindowSize(ImVec2(
			IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y
		));

		// Cast.
		const auto openedEvent = reinterpret_cast<EventOnAnimatorWindowOpened*>(e);
		
		// Check controller is nullptr.
		if (openedEvent->GetController())
		{
			_controller = openedEvent->GetController();
		}
	}

	void EditorAnimatorWindow::OnClosed([[maybe_unused]] Event* e)
	{
		_open = false;
		_controller = nullptr;
	}

	void EditorAnimatorWindow::AnimationClipAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::AnimationClip, "Data type must be animation clip!");

		const auto clipData = static_cast<Utils::DragAndDropAnimationClipData*>(data);

		if (!clipData->Get())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Animation clip not found!";

			return;
		}

		_controller->Add(clipData->Get());
	}

	void EditorAnimatorWindow::ParameterWindow()
	{
		const auto regionAvail = ImVec2(ImGui::GetContentRegionAvail().x * (1.0f / 3.0f), ImGui::GetContentRegionAvail().y);
		if (ImGui::BeginChild("Parameter", regionAvail, true))
		{
			if (ImGui::BeginTabBar("Areas##EditorAnimatorWindow", ImGuiTabBarFlags_FittingPolicyResizeDown))
			{
				if (ImGui::BeginTabItem("Parameter##Areas"))
				{
					if (ImGui::TabItemButton("+##Parameter"))
					{
						_addParameterButtonPressed = true;
					}

					// Display list.
					if (_addParameterButtonPressed)
					{
						if (ImGui::BeginPopupContextItem("##ParameterValueType", ImGuiPopupFlags_MouseButtonLeft))
						{
							if (ImGui::Selectable("Float##ParameterValueType"))
							{
								AnimatorParameter param = { };
								param.name = _controller->ObtainProceedName("Float");
								param.value.valueFloat = 0.0f;
								param.value.valueType = AnimatorValue::Type::Float;

								_controller->AddParameter(param);

								_addParameterButtonPressed = false;
							}

							if (ImGui::Selectable("Int##ParameterValueType"))
							{
								AnimatorParameter param = { };
								param.name = _controller->ObtainProceedName("Int");
								param.value.valueInt = 0;
								param.value.valueType = AnimatorValue::Type::Int;

								_controller->AddParameter(param);

								_addParameterButtonPressed = false;

							}

							if (ImGui::Selectable("Bool##ParameterValueType"))
							{
								AnimatorParameter param = { };
								param.name = _controller->ObtainProceedName("Bool");
								param.value.valueBool = false;
								param.value.valueType = AnimatorValue::Type::Bool;

								_controller->AddParameter(param);

								_addParameterButtonPressed = false;

							}

							if (ImGui::Selectable("Trigger##ParameterValueType"))
							{
								AnimatorParameter param = { };
								param.name = _controller->ObtainProceedName("Trigger");
								param.value.valueTrigger = false;
								param.value.valueType = AnimatorValue::Type::Trigger;

								_controller->AddParameter(param);

								_addParameterButtonPressed = false;
							}

							ImGui::EndPopup();
						}
					}

					_controller->OnParameterGui();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			// Reset add parameter button pressed.
			if (ImGui::IsItemClicked())
			{
				_addParameterButtonPressed = false;
			}

			ImGui::EndChild();
		}
	}

	void EditorAnimatorWindow::SequenceWindow()
	{
		const auto regionAvail = ImVec2(ImGui::GetContentRegionAvail().x * (1.0f / 2.0f), ImGui::GetContentRegionAvail().y);
		if (ImGui::BeginChild("Sequence", regionAvail, true))
		{
			if (ImGui::BeginTabBar("Sequence##EditorAnimatorWindow", ImGuiTabBarFlags_FittingPolicyResizeDown))
			{
				if (ImGui::BeginTabItem("Sequence##Areas"))
				{
					// Drag and drop target.
					{
						const auto dragAndDropArea = ImVec2(regionAvail.x, regionAvail.y / 10.0f);

						ImGui::Text("Drag and drop here!");
						ImGui::InvisibleButton("##DragAndDrop", dragAndDropArea);

						const DragAndDropSpecification specification =
						{
							"AnimationClip", sizeof(Utils::DragAndDropAnimationClipData),
							nullptr, DragAndDropDataType::AnimationClip,
							DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(AnimationClipAssetDragAndDropTargetCallback),
							DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
						};
						DragAndDropTarget dragAndDropTarget(specification);
					}

					ImGui::Separator();

					_controller->OnSequenceGui();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::EndChild();
		}
	}

	void EditorAnimatorWindow::InspectorWindow()
	{
		const auto regionAvail = ImGui::GetContentRegionAvail();
		if (ImGui::BeginChild("Inspector", regionAvail, true))
		{
			if (ImGui::BeginTabBar("Inspector##EditorAnimatorWindow", ImGuiTabBarFlags_FittingPolicyResizeDown))
			{
				if (ImGui::BeginTabItem("Inspector##Areas"))
				{
					_controller->OnInspectorGui();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::EndChild();
		}
	}

	void EditorAnimatorWindow::Restore()
	{

	}
}
