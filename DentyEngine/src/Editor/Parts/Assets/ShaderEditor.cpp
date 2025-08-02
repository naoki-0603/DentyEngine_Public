#include "stdafx.h"

#include "ShaderEditor.h"

#include "Graphics/Renderer/Shader.h"

#include "System/Events/EventEditor.h"

#include "System/Core/Constants.h"
#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	ShaderEditor::ShaderEditor() :
		IEditor(), AssetEditor(), _shader()
	{
	}

	void ShaderEditor::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnShaderEditorOpened>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventOnShaderEditorClosed>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void ShaderEditor::OnGui(Ref<Scene> activeScene)
	{
		// 	If the window is open
		if (not _open) return;

		if (ImGui::Begin("ShaderEditor##AssetEditor", &_open, IMGUI_DEFAULT_WINDOW_FLAGS))
		{
			// ShaderID
			ImGui::Text("ShaderID: %u", _shader->GetShaderID());

			// Used shader file paths.
			const std::string label = std::string("Shaders") + "##" + std::to_string(_shader->GetShaderID());
			if (ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed))
			{
				const ShaderPathHolder& pathHolder = _shader->GetShaderPathHolder();

				ImGui::Text("VS: %s", pathHolder.vertexShaderFilePath.c_str());
				ImGui::Text("HS: %s", pathHolder.hullShaderFilePath.c_str());
				ImGui::Text("DS: %s", pathHolder.domainShaderFilePath.c_str());
				ImGui::Text("GS: %s", pathHolder.geometryShaderFilePath.c_str());
				ImGui::Text("PS: %s", pathHolder.pixelShaderFilePath.c_str());

				for (int index = 0; index < pathHolder.computeShaderFilePaths.size(); ++index)
				{
					ImGui::Text("CS0%d: %s", index, pathHolder.computeShaderFilePaths.at(index).c_str());
				}

				ImGui::TreePop();
			}

			ImGui::Separator();

			// ReCompile shaders.
			if (GuiUtils::ButtonCentered("ReCompile##ShaderEditor"))
			{
				_shader->ReCompile();
			}


		}

		ImGui::End();

		// If closed.
		if (not _open)
		{
			EventOnShaderEditorClosed eventOnShaderEditorClosed;
			NOTIFY_EVENT(eventOnShaderEditorClosed);
		}
	}

	void ShaderEditor::OnOpened(EventOnShaderEditorOpened* e)
	{
		_open = true;
		_shader = e->GetShader();

		ImGui::SetNextWindowSize(ImVec2(IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y));
	}

	void ShaderEditor::OnClosed([[maybe_unused]] EventOnShaderEditorClosed* e)
	{
		_open = false;
		_shader = nullptr;
	}
}
