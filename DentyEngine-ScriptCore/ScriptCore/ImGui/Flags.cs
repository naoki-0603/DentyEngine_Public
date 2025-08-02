using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// Copy from imgui.h
namespace DentyEngine
{
    public enum ImGuiTreeNodeFlags : int
    {
        ImGuiTreeNodeFlags_None                 = 0,
        ImGuiTreeNodeFlags_Selected             = 1 << 0,   // Draw as selected
        ImGuiTreeNodeFlags_Framed               = 1 << 1,   // Draw frame with background (e.g. for CollapsingHeader)
        ImGuiTreeNodeFlags_AllowItemOverlap     = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
        ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
        ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
        ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,   // Default node to be open
        ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Need double-click to open node
        ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,   // Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
        ImGuiTreeNodeFlags_Leaf                 = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
        ImGuiTreeNodeFlags_Bullet               = 1 << 9,   // Display a bullet instead of arrow
        ImGuiTreeNodeFlags_FramePadding         = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
        ImGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.
        ImGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend hit box to the left-most and right-most edges (bypass the indented area).
        ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
        //ImGuiTreeNodeFlags_NoScrollOnOpen     = 1 << 14,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
        ImGuiTreeNodeFlags_CollapsingHeader     = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog,
    }

    public enum ImGuiStyleVar : int
    {
        // Enum name --------------------- // Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
        ImGuiStyleVar_Alpha,               // float     Alpha
        ImGuiStyleVar_DisabledAlpha,       // float     DisabledAlpha
        ImGuiStyleVar_WindowPadding,       // ImVec2    WindowPadding
        ImGuiStyleVar_WindowRounding,      // float     WindowRounding
        ImGuiStyleVar_WindowBorderSize,    // float     WindowBorderSize
        ImGuiStyleVar_WindowMinSize,       // ImVec2    WindowMinSize
        ImGuiStyleVar_WindowTitleAlign,    // ImVec2    WindowTitleAlign
        ImGuiStyleVar_ChildRounding,       // float     ChildRounding
        ImGuiStyleVar_ChildBorderSize,     // float     ChildBorderSize
        ImGuiStyleVar_PopupRounding,       // float     PopupRounding
        ImGuiStyleVar_PopupBorderSize,     // float     PopupBorderSize
        ImGuiStyleVar_FramePadding,        // ImVec2    FramePadding
        ImGuiStyleVar_FrameRounding,       // float     FrameRounding
        ImGuiStyleVar_FrameBorderSize,     // float     FrameBorderSize
        ImGuiStyleVar_ItemSpacing,         // ImVec2    ItemSpacing
        ImGuiStyleVar_ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
        ImGuiStyleVar_IndentSpacing,       // float     IndentSpacing
        ImGuiStyleVar_CellPadding,         // ImVec2    CellPadding
        ImGuiStyleVar_ScrollbarSize,       // float     ScrollbarSize
        ImGuiStyleVar_ScrollbarRounding,   // float     ScrollbarRounding
        ImGuiStyleVar_GrabMinSize,         // float     GrabMinSize
        ImGuiStyleVar_GrabRounding,        // float     GrabRounding
        ImGuiStyleVar_TabRounding,         // float     TabRounding
        ImGuiStyleVar_ButtonTextAlign,     // ImVec2    ButtonTextAlign
        ImGuiStyleVar_SelectableTextAlign, // ImVec2    SelectableTextAlign
        ImGuiStyleVar_COUNT
    }

    public enum ImGuiCol : int
    {
        ImGuiCol_Text,
        ImGuiCol_TextDisabled,
        ImGuiCol_WindowBg,              // Background of normal windows
        ImGuiCol_ChildBg,               // Background of child windows
        ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
        ImGuiCol_Border,
        ImGuiCol_BorderShadow,
        ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
        ImGuiCol_FrameBgHovered,
        ImGuiCol_FrameBgActive,
        ImGuiCol_TitleBg,
        ImGuiCol_TitleBgActive,
        ImGuiCol_TitleBgCollapsed,
        ImGuiCol_MenuBarBg,
        ImGuiCol_ScrollbarBg,
        ImGuiCol_ScrollbarGrab,
        ImGuiCol_ScrollbarGrabHovered,
        ImGuiCol_ScrollbarGrabActive,
        ImGuiCol_CheckMark,
        ImGuiCol_SliderGrab,
        ImGuiCol_SliderGrabActive,
        ImGuiCol_Button,
        ImGuiCol_ButtonHovered,
        ImGuiCol_ButtonActive,
        ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
        ImGuiCol_HeaderHovered,
        ImGuiCol_HeaderActive,
        ImGuiCol_Separator,
        ImGuiCol_SeparatorHovered,
        ImGuiCol_SeparatorActive,
        ImGuiCol_ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
        ImGuiCol_ResizeGripHovered,
        ImGuiCol_ResizeGripActive,
        ImGuiCol_Tab,                   // TabItem in a TabBar
        ImGuiCol_TabHovered,
        ImGuiCol_TabActive,
        ImGuiCol_TabUnfocused,
        ImGuiCol_TabUnfocusedActive,
        ImGuiCol_DockingPreview,        // Preview overlay color when about to docking something
        ImGuiCol_DockingEmptyBg,        // Background color for empty node (e.g. CentralNode with no window docked into it)
        ImGuiCol_PlotLines,
        ImGuiCol_PlotLinesHovered,
        ImGuiCol_PlotHistogram,
        ImGuiCol_PlotHistogramHovered,
        ImGuiCol_TableHeaderBg,         // Table header background
        ImGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
        ImGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
        ImGuiCol_TableRowBg,            // Table row background (even rows)
        ImGuiCol_TableRowBgAlt,         // Table row background (odd rows)
        ImGuiCol_TextSelectedBg,
        ImGuiCol_DragDropTarget,        // Rectangle highlighting a drop target
        ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
        ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
        ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
        ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
        ImGuiCol_COUNT
    }

    public enum ImGuiDragAndDropType32 : int
    {
        ImGuiDragAndDropType32_Material = 0,
        ImGuiDragAndDropType32_Sprite = 13,
        ImGuiDragAndDropType32_GameObject = 100,
        ImGuiDragAndDropType32_COUNT
    }

    public enum ImGuiDragAndDropType64 : int
    {
        ImGuiDragAndDropType64_Texture = 5,
        ImGuiDragAndDropType64_Prefab = 6,
        ImGuiDragAndDropType64_COUNT
    }
}
