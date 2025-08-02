#include "stdafx.h"

#include "EditorToolBar.h"

#include "System/Core/Window.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Events/EventEditor.h"

#include "System/Layer/ApplicationLayer.h"

#include "System/Scene/Scene.h"

namespace DentyEngine
{
	EditorToolBar::EditorToolBar([[maybe_unused]] ID3D11Device* device) :
		_open(true)
	{
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		_playButton = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::TOOLBAR_PLAY_TEXTURE_FILE_PATH).value();
		_pauseButton = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::TOOLBAR_PAUSE_TEXTURE_FILE_PATH).value();
		_stepButton = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::TOOLBAR_STEP_TEXTURE_FILE_PATH).value();
		_resumeButton = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::TOOLBAR_RESUME_TEXTURE_FILE_PATH).value();

		_buttonSize = Vector2(20.064f, 15);
		_offsetFromStart = 900.0f;
		_gapBetweenButton = 29.832f;
	}

	void EditorToolBar::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void EditorToolBar::OnGui(Ref<Scene> activeScene)
	{
		ImGuiWindowFlags windowFlags = {  };
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoDecoration;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;

		const Vector2 size = Window::GetInfo().GetSize<Vector2>();

		if (ImGui::Begin("ToolBar##Editor", nullptr, windowFlags))
		{
			ImGui::SameLine(_offsetFromStart - _gapBetweenButton);

			// Get current state.
			const bool isPlay = ApplicationLayer::IsPlay();
			const bool isPause = ApplicationLayer::IsPause();
			[[maybe_unused]] const bool isStep = ApplicationLayer::IsStep();

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.32f, 0.32f, 0.32f, 1.0f));

			// Play button.
			{
				if (isPlay || isPause)
				{
				    // Set dark gray.
					constexpr float darkGrayColorRate = 0.4f;
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(darkGrayColorRate, darkGrayColorRate, darkGrayColorRate, 1.0f));
				}

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
				{
					if (isPause)
					{
						const ImTextureID textureID = *_resumeButton->GetResource();

						if (ImGui::ImageButton("ResumeButton", textureID, _buttonSize.ToImVec2()))
						{
							EventOnSceneResumeButtonPressed eventOnSceneResumeButtonPressed;
							NOTIFY_EVENT(eventOnSceneResumeButtonPressed);
						}
					}
					else
					{
						const ImTextureID textureID = *_playButton->GetResource();

						if (ImGui::ImageButton("PlayButton", textureID, _buttonSize.ToImVec2()))
						{
							bool isFileExists = activeScene->GetFilePathToSerialized().IsValid();
							if (isFileExists)
							{
								EventOnScenePlayButtonPressed eventOnScenePlayButtonPressed;
								NOTIFY_EVENT(eventOnScenePlayButtonPressed);
							}
							else
							{
								DENTY_ERR_CONSOLE_LOG("Scene file doesn't exists");
								DENTY_ERR_CONSOLE_LOG("Try save scene");
							}

						}
					}

				    ImGui::SameLine(_offsetFromStart);
				}
				ImGui::PopStyleVar();

				if (isPlay || isPause)
				{
					ImGui::PopStyleColor();
				}
			}

			// Pause button.
			{
				if (isPause)
				{
					// Set dark gray.
					constexpr float darkGrayColorRate = 0.4f;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(darkGrayColorRate, darkGrayColorRate, darkGrayColorRate, 1.0f));
				}

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.5f);
				{
				    const ImTextureID textureID = *_pauseButton->GetResource();

				    if (ImGui::ImageButton("PauseButton", textureID, _buttonSize.ToImVec2()))
				    {
						// Enable when play state. (or step?)
						if (isPlay)
						{
							EventOnScenePauseButtonPressed eventOnScenePauseButtonPressed = { };
							NOTIFY_EVENT(eventOnScenePauseButtonPressed);
						}
						else
						{
							DENTY_WARN_CONSOLE_LOG("Enable when play state.");
						}
				    }

				    ImGui::SameLine(_offsetFromStart + _gapBetweenButton);
				}
				ImGui::PopStyleVar();

				if (isPause)
				{
					ImGui::PopStyleColor();
				}
			}

			// Step button.
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
				{
				    const ImTextureID textureID = *_stepButton->GetResource();

				    if (ImGui::ImageButton("StepButton", textureID, _buttonSize.ToImVec2()))
				    {
						DENTY_WARN_CONSOLE_LOG("Doesn't implemented yet.");
				    }
				}
				ImGui::PopStyleVar();
			}

			ImGui::PopStyleColor(2);

		}

		ImGui::End();
	}
};