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

		// Mangenta is only used for elements which are not yet relevant in the editor, so I know
		Mangenta
	};

	inline constexpr ImVec4 ColorToImVec4(const DefaultColors color)
	{
		switch (color)
		{
		case Darkest:
			return { 0.086f, 0.086f, 0.086f, 1.0f };
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
		case Mangenta:
			return { 1.0f, 0.0f, 1.0f, 1.0f };
		default:
			return { 1.0f, 1.0f, 1.0f, 1.0f };;
			break;
		}
	}

	struct DuskImGuiColors
	{
		ImVec4 Text						= ColorToImVec4(White);
		ImVec4 TextDisabled				= ColorToImVec4(Grey3);
		ImVec4 WindowBg					= ColorToImVec4(Grey2);
		ImVec4 ChildBg					= ColorToImVec4(Dark);
		ImVec4 PopupBg					= ColorToImVec4(Darkest);
		ImVec4 Border					= ColorToImVec4(Dark);
		ImVec4 BorderShadow				= ColorToImVec4(Dark);
		ImVec4 FrameBg					= ColorToImVec4(Grey1);
		ImVec4 FrameBgHovered			= ColorToImVec4(Mangenta);
		ImVec4 FrameBgActive			= ColorToImVec4(Mangenta);
		ImVec4 TitleBg					= ColorToImVec4(Dark);
		ImVec4 TitleBgActive			= ColorToImVec4(Dark);
		ImVec4 TitleBgCollapsed			= ColorToImVec4(Dark);
		ImVec4 MenuBarBg				= ColorToImVec4(Dark);
		ImVec4 ScrollbarBg				= ColorToImVec4(Mangenta);
		ImVec4 ScrollbarGrab			= ColorToImVec4(Mangenta);
		ImVec4 ScrollbarGrabHovered		= ColorToImVec4(Mangenta);
		ImVec4 ScrollbarGrabActive		= ColorToImVec4(Mangenta);
		ImVec4 CheckMark				= ColorToImVec4(Mangenta);
		ImVec4 SliderGrab				= ColorToImVec4(Mangenta);
		ImVec4 SliderGrabActive			= ColorToImVec4(Mangenta);
		ImVec4 Button					= ColorToImVec4(Grey1);
		ImVec4 ButtonHovered			= ColorToImVec4(Grey2);
		ImVec4 ButtonActive				= ColorToImVec4(Grey3);
		ImVec4 Header					= ColorToImVec4(Dark);
		ImVec4 HeaderHovered			= ColorToImVec4(Dark);
		ImVec4 HeaderActive				= ColorToImVec4(Dark);
		ImVec4 Separator				= ColorToImVec4(Dark);
		ImVec4 SeparatorHovered			= ColorToImVec4(Dark);
		ImVec4 SeparatorActive			= ColorToImVec4(Dark);
		ImVec4 ResizeGrip				= ColorToImVec4(Mangenta);
		ImVec4 ResizeGripHovered		= ColorToImVec4(Mangenta);
		ImVec4 ResizeGripActive			= ColorToImVec4(Mangenta);
		ImVec4 Tab						= ColorToImVec4(Grey1);
		ImVec4 TabHovered				= ColorToImVec4(Grey3);
		ImVec4 TabActive				= ColorToImVec4(Grey3);
		ImVec4 TabUnfocused				= ColorToImVec4(Grey1);
		ImVec4 TabUnfocusedActive		= ColorToImVec4(Grey2);
		ImVec4 DockingPreview			= ColorToImVec4(Mangenta);
		ImVec4 DockingEmptyBg			= ColorToImVec4(Mangenta);
		ImVec4 PlotLines				= ColorToImVec4(Mangenta);
		ImVec4 PlotLinesHovered			= ColorToImVec4(Mangenta);
		ImVec4 PlotHistogram			= ColorToImVec4(Mangenta);
		ImVec4 PlotHistogramHovered		= ColorToImVec4(Mangenta);
		ImVec4 TableHeaderBg			= ColorToImVec4(Mangenta);
		ImVec4 TableBorderStrong		= ColorToImVec4(Mangenta);
		ImVec4 TableBorderLight			= ColorToImVec4(Mangenta);
		ImVec4 TableRowBg				= ColorToImVec4(Mangenta);
		ImVec4 TableRowBgAlt			= ColorToImVec4(Mangenta);
		ImVec4 TextSelectedBg			= ColorToImVec4(Mangenta);
		ImVec4 DragDropTarget			= ColorToImVec4(Mangenta);
		ImVec4 NavHighlight				= ColorToImVec4(Mangenta);
		ImVec4 NavWindowingHighlight	= ColorToImVec4(Mangenta);
		ImVec4 NavWindowingDimBg		= ColorToImVec4(Mangenta);
		ImVec4 ModalWindowDimBg			= ColorToImVec4(Mangenta);
	};

	struct DuskImGuiVars
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

	struct DuskImGuiStyle
	{
		DuskImGuiColors Colors;
		DuskImGuiVars Vars;
	};
}
