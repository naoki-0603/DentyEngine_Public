#include "stdafx.h"

#include "ImGuiUtilities.h"

#include "System/Core/DString.h"

namespace DentyEngine
{
	namespace GuiUtils
	{
		// Return is selected same item, if same return true, otherwise return false.
		bool ComboBoxGui(const std::string& label, const std::string* items, int itemSize, std::string& previousValue, ImGuiComboFlags flags)
		{
			bool isSelectedSameItem = true;
			if (ImGui::BeginCombo(label.c_str(), previousValue.c_str(), flags))
			{
				bool isSelected = false;
				for (int index = 0; index < itemSize; ++index)
				{
					if (ImGui::Selectable(items[index].c_str(), &isSelected))
					{
						if (previousValue != items[index])
						{
							isSelectedSameItem = false;
						}

						previousValue = items[index];
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			return isSelectedSameItem;
		}

		bool ButtonCentered(std::string_view text)
		{
			std::string string(text);

			const auto windowWidth = ImGui::GetWindowSize().x;
			const auto textWidth = ImGui::CalcTextSize(string.c_str()).x;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			return ImGui::Button(string.c_str());
		}

		bool ButtonCentered(std::string_view text, const Vector2& size)
		{
			std::string string(text);

			const auto windowWidth = ImGui::GetWindowSize().x;

			ImGui::SetCursorPosX((windowWidth - size.x) * 0.5f);
			return ImGui::Button(string.c_str(), ImVec2(size.x, size.y));
		}

		bool InputFileName(std::string& fileName)
		{
			bool isEditing = false;
			if (ImGui::BeginPopupContextItem("##InputFileName"))
			{
				isEditing = String::InputTextGui("##InputTextFileName", &fileName);

				ImGui::EndPopup();
			}

			return isEditing;
		}

		void HelpMarker(const char* desc)
		{
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(desc);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		void DisplayMatrix(const Matrix& matrix)
		{
			ImGui::Text("%f, %f, %f, %f", matrix._11, matrix._12, matrix._13, matrix._14);
			ImGui::Text("%f, %f, %f, %f", matrix._21, matrix._22, matrix._23, matrix._24);
			ImGui::Text("%f, %f, %f, %f", matrix._31, matrix._32, matrix._33, matrix._34);
			ImGui::Text("%f, %f, %f, %f", matrix._41, matrix._42, matrix._43, matrix._44);
		}

		ConfirmGuiState ConfirmGui(std::string_view text, const Vector2& position)
		{
			auto state = ConfirmGuiState::None;

			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_AlwaysAutoResize;

			bool open = true;
			ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
			if (ImGui::Begin("Confirm##ConfirmationWindow", &open, windowFlags))
			{
				const auto t = std::string(text);
				String::TextCentered(t);

				if (ImGui::Button("Yes"))
				{
					state = ConfirmGuiState::Yes;
				}

				ImGui::SameLine();

				if (ImGui::Button("No"))
				{
					state = ConfirmGuiState::No;
				}

			}

			ImGui::End();

			return state;
		}
	}
};