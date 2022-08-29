#pragma once

#include "imgui/imgui.h"

namespace DuskEngine
{
	enum DefaultColors
	{
		Darkest,
		Dark,
		Grey1,
		Grey2,
		Grey3,
		Grey4,
		White,
	};

	inline constexpr ImVec4 ColorToImVec4(const DefaultColors color)
	{
		switch (color)
		{
		case Darkest:
			return { 0.08f, 0.08f, 0.08f, 1.0f };
			break;
		case Dark:
			return { 0.12f, 0.12f, 0.12f, 1.0f };
			break;
		case Grey1:
			return { 0.15f, 0.15f, 0.15f, 1.0f };
			break;
		case Grey2:
			return { 0.20f, 0.20f, 0.20f, 1.0f };
			break;
		case Grey3:
			return { 0.25f, 0.25f, 0.25f, 1.0f };
			break;
		case Grey4:
			return { 0.41f, 0.50f, 0.52f, 1.0f };
			break;
		case White:
			return { 0.87f, 0.87f, 0.87f, 1.0f };
			break;
		default:
			return { 1.00f, 0.00f, 1.00f, 1.0f };
			break;
		}
	}

	struct _DefaultColors
	{
		const ImVec4 Darkest	= { 0.08f, 0.08f, 0.08f, 1.0f };
		const ImVec4 Dark		= { 0.12f, 0.12f, 0.12f, 1.0f };
		const ImVec4 Grey1		= { 0.15f, 0.15f, 0.15f, 1.0f };
		const ImVec4 Grey2		= { 0.20f, 0.20f, 0.20f, 1.0f };
		const ImVec4 Grey3		= { 0.25f, 0.25f, 0.25f, 1.0f };
		const ImVec4 Grey4		= { 0.41f, 0.50f, 0.52f, 1.0f };
		const ImVec4 White		= { 0.87f, 0.87f, 0.87f, 1.0f };
	};

	struct _Colors
	{
		ImVec4 ImGuiCol_Text					= ColorToImVec4(White);
		ImVec4 ImGuiCol_TextDisabled			= ColorToImVec4(White);
		ImVec4 ImGuiCol_WindowBg				= ColorToImVec4(White);
		ImVec4 ImGuiCol_ChildBg					= ColorToImVec4(White);
		ImVec4 ImGuiCol_PopupBg					= ColorToImVec4(White);
		ImVec4 ImGuiCol_Border					= ColorToImVec4(White);
		ImVec4 ImGuiCol_BorderShadow			= ColorToImVec4(White);
		ImVec4 ImGuiCol_FrameBg					= ColorToImVec4(White);
		ImVec4 ImGuiCol_FrameBgHovered			= ColorToImVec4(White);
		ImVec4 ImGuiCol_FrameBgActive			= ColorToImVec4(White);
		ImVec4 ImGuiCol_TitleBg					= ColorToImVec4(White);
		ImVec4 ImGuiCol_TitleBgActive			= ColorToImVec4(White);
		ImVec4 ImGuiCol_TitleBgCollapsed		= ColorToImVec4(White);
		ImVec4 ImGuiCol_MenuBarBg				= ColorToImVec4(White);
		ImVec4 ImGuiCol_ScrollbarBg				= ColorToImVec4(White);
		ImVec4 ImGuiCol_ScrollbarGrab			= ColorToImVec4(White);
		ImVec4 ImGuiCol_ScrollbarGrabHovered	= ColorToImVec4(White);
		ImVec4 ImGuiCol_ScrollbarGrabActive		= ColorToImVec4(White);
		ImVec4 ImGuiCol_CheckMark				= ColorToImVec4(White);
		ImVec4 ImGuiCol_SliderGrab				= ColorToImVec4(White);
		ImVec4 ImGuiCol_SliderGrabActive		= ColorToImVec4(White);
		ImVec4 ImGuiCol_Button					= ColorToImVec4(White);
		ImVec4 ImGuiCol_ButtonHovered			= ColorToImVec4(White);
		ImVec4 ImGuiCol_ButtonActive			= ColorToImVec4(White);
		ImVec4 ImGuiCol_Header					= ColorToImVec4(White);
		ImVec4 ImGuiCol_HeaderHovered			= ColorToImVec4(White);
		ImVec4 ImGuiCol_HeaderActive			= ColorToImVec4(White);
		ImVec4 ImGuiCol_Separator				= ColorToImVec4(White);
		ImVec4 ImGuiCol_SeparatorHovered		= ColorToImVec4(White);
		ImVec4 ImGuiCol_SeparatorActive			= ColorToImVec4(White);
		ImVec4 ImGuiCol_ResizeGrip				= ColorToImVec4(White);
		ImVec4 ImGuiCol_ResizeGripHovered		= ColorToImVec4(White);
		ImVec4 ImGuiCol_ResizeGripActive		= ColorToImVec4(White);
		ImVec4 ImGuiCol_Tab						= ColorToImVec4(White);
		ImVec4 ImGuiCol_TabHovered				= ColorToImVec4(White);
		ImVec4 ImGuiCol_TabActive				= ColorToImVec4(White);
		ImVec4 ImGuiCol_TabUnfocused			= ColorToImVec4(White);
		ImVec4 ImGuiCol_TabUnfocusedActive		= ColorToImVec4(White);
		ImVec4 ImGuiCol_DockingPreview			= ColorToImVec4(White);
		ImVec4 ImGuiCol_DockingEmptyBg			= ColorToImVec4(White);
		ImVec4 ImGuiCol_PlotLines				= ColorToImVec4(White);
		ImVec4 ImGuiCol_PlotLinesHovered		= ColorToImVec4(White);
		ImVec4 ImGuiCol_PlotHistogram			= ColorToImVec4(White);
		ImVec4 ImGuiCol_PlotHistogramHovered	= ColorToImVec4(White);
		ImVec4 ImGuiCol_TableHeaderBg			= ColorToImVec4(White);
		ImVec4 ImGuiCol_TableBorderStrong		= ColorToImVec4(White);
		ImVec4 ImGuiCol_TableBorderLight		= ColorToImVec4(White);
		ImVec4 ImGuiCol_TableRowBg				= ColorToImVec4(White);
		ImVec4 ImGuiCol_TableRowBgAlt			= ColorToImVec4(White);
		ImVec4 ImGuiCol_TextSelectedBg			= ColorToImVec4(White);
		ImVec4 ImGuiCol_DragDropTarget			= ColorToImVec4(White);
		ImVec4 ImGuiCol_NavHighlight			= ColorToImVec4(White);
		ImVec4 ImGuiCol_NavWindowingHighlight	= ColorToImVec4(White);
		ImVec4 ImGuiCol_NavWindowingDimBg		= ColorToImVec4(White);
		ImVec4 ImGuiCol_ModalWindowDimBg		= ColorToImVec4(White);
	};

	struct _Vars
	{
		float       Alpha;
		float       DisabledAlpha;
		ImVec2      WindowPadding;
		float       WindowRounding;
		float       WindowBorderSize;
		ImVec2      WindowMinSize;
		ImVec2      WindowTitleAlign;
		ImGuiDir    WindowMenuButtonPosition;
		float       ChildRounding;
		float       ChildBorderSize;
		float       PopupRounding;
		float       PopupBorderSize;
		ImVec2      FramePadding;
		float       FrameRounding;
		float       FrameBorderSize;
		ImVec2      ItemSpacing;
		ImVec2      ItemInnerSpacing;
		ImVec2      CellPadding;
		ImVec2      TouchExtraPadding;
		float       IndentSpacing;
		float       ColumnsMinSpacing;
		float       ScrollbarSize;
		float       ScrollbarRounding;
		float       GrabMinSize;
		float       GrabRounding;
		float       LogSliderDeadzone;
		float       TabRounding;
		float       TabBorderSize;
		float       TabMinWidthForCloseButton;
		ImGuiDir    ColorButtonPosition;
		ImVec2      ButtonTextAlign;
		ImVec2      SelectableTextAlign;
		ImVec2      DisplayWindowPadding;
		ImVec2      DisplaySafeAreaPadding;
		float       MouseCursorScale;
		bool        AntiAliasedLines;
		bool        AntiAliasedLinesUseTex;
		bool        AntiAliasedFill;
		float       CurveTessellationTol;
		float       CircleTessellationMaxError;
	};

	struct Style
	{
		_DefaultColors DefaultColors;
		_Colors Colors;
		//_Vars Vars;
	};

}
