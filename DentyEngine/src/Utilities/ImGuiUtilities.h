#pragma once

namespace DentyEngine
{
    namespace GuiUtils
    {
        enum class ConfirmGuiState;

        extern bool ComboBoxGui(const std::string& label, const std::string* items, int itemSize, std::string& previousValue, ImGuiComboFlags flags = 0);

        // Return is button pressed.
        extern bool ButtonCentered(std::string_view text);
        extern bool ButtonCentered(std::string_view text, const Vector2& size);

        // Return is editing.
        extern bool InputFileName(std::string& fileName);

        // Copy from imgui_demo.cpp
        extern void HelpMarker(const char* desc);

        extern void DisplayMatrix(const Matrix& matrix);

        extern ConfirmGuiState ConfirmGui(std::string_view text, const Vector2& position);

        enum class ConfirmGuiState
        {
            None,
            Yes,
            No,

            Max
        };
    }

   
};