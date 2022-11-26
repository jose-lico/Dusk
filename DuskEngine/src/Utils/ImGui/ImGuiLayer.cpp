#include "pch.h"
#include "ImGuiLayer.h"

#include "DuskImGuiStyle.h"
#include "Core/Events/Events.h"
#include "Core/Application/Window.h"
#include "Utils/Profiling/Timer.h"

#include "IconsForkAwesome.h"
#include "ImGuizmo/ImGuizmo.h"
#include "imgui/imgui.h"
#include "../backends/imgui_impl_opengl3.cpp"
#include "../backends/imgui_impl_glfw.cpp"

#include "fonts/roboto/Roboto-Regular.ttf.embedded"
#include "fonts/forkawesome/forkawesome-webfont.ttf.embedded"

namespace DuskEngine
{
	const ImWchar glyphRanges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };

	ImGuiLayer::ImGuiLayer()
		:m_Style(new DuskImGuiStyle())
	{
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = "teste.ini";
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMenuButtonPosition = ImGuiDir_None;

		AddFontFromMemory((void*)EMBEDDED_ROBOTO_REGULAR, 18.0f);

		ApplyStyle();
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool* showDemoWindowPtr;
		showDemoWindowPtr = &m_ShowDemoWindow;
		if (m_ShowDemoWindow) ImGui::ShowDemoWindow(showDemoWindowPtr);
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::ActivateDemoWindow));
	}

	void ImGuiLayer::SetGLContext(Window* window)
	{
		m_Window = window;

		ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::DestroyGLContext()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void ImGuiLayer::Begin() const
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End() const
	{
		ImGuiIO& io = ImGui::GetIO();
		int32_t width;
		int32_t height;
		glfwGetWindowSize(m_Window->GetNativeHandle(), &width, &height);
		io.DisplaySize = ImVec2((float)width, (float)height);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::AddFontFromFile(const std::string& path, float size) const
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.Fonts->AddFontFromFileTTF(path.c_str(), size);

		ImFontConfig fontConfig;
		fontConfig.PixelSnapH = true;
		fontConfig.MergeMode = true;
		fontConfig.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromMemoryTTF((void*)EMBEDDED_FORKAWESOME_WEBFONT, (int32_t)sizeof(EMBEDDED_FORKAWESOME_WEBFONT), size, &fontConfig, glyphRanges);
	}

	void ImGuiLayer::AddFontFromMemory(void* fontData, float size) const
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromMemoryTTF(fontData, sizeof(EMBEDDED_ROBOTO_REGULAR), size, &fontConfig);

		fontConfig.PixelSnapH = true;
		fontConfig.MergeMode = true;
		
		io.Fonts->AddFontFromMemoryTTF((void*)EMBEDDED_FORKAWESOME_WEBFONT, (int32_t)sizeof(EMBEDDED_FORKAWESOME_WEBFONT), size, &fontConfig, glyphRanges);
	}

	bool ImGuiLayer::ActivateDemoWindow(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::TAB)
			m_ShowDemoWindow = !m_ShowDemoWindow;

		return true;
	}

	void ImGuiLayer::ApplyStyle() const
	{
		ImGuiStyle& style = ImGui::GetStyle();
		
		style.Colors[ImGuiCol_Text                 ] = m_Style->Colors.Text;
		style.Colors[ImGuiCol_TextDisabled		   ] = m_Style->Colors.TextDisabled;
		style.Colors[ImGuiCol_WindowBg			   ] = m_Style->Colors.WindowBg;
		style.Colors[ImGuiCol_ChildBg			   ] = m_Style->Colors.ChildBg;
		style.Colors[ImGuiCol_PopupBg			   ] = m_Style->Colors.PopupBg;
		style.Colors[ImGuiCol_Border			   ] = m_Style->Colors.Border;
		style.Colors[ImGuiCol_BorderShadow		   ] = m_Style->Colors.BorderShadow;
		style.Colors[ImGuiCol_FrameBg			   ] = m_Style->Colors.FrameBg;
		style.Colors[ImGuiCol_FrameBgHovered	   ] = m_Style->Colors.FrameBgHovered;
		style.Colors[ImGuiCol_FrameBgActive		   ] = m_Style->Colors.FrameBgActive;
		style.Colors[ImGuiCol_TitleBg			   ] = m_Style->Colors.TitleBg;
		style.Colors[ImGuiCol_TitleBgActive		   ] = m_Style->Colors.TitleBgActive;
		style.Colors[ImGuiCol_TitleBgCollapsed	   ] = m_Style->Colors.TitleBgCollapsed;
		style.Colors[ImGuiCol_MenuBarBg			   ] = m_Style->Colors.MenuBarBg;
		style.Colors[ImGuiCol_ScrollbarBg		   ] = m_Style->Colors.ScrollbarBg;
		style.Colors[ImGuiCol_ScrollbarGrab		   ] = m_Style->Colors.ScrollbarGrab;
		style.Colors[ImGuiCol_ScrollbarGrabHovered ] = m_Style->Colors.ScrollbarGrabHovered;
		style.Colors[ImGuiCol_ScrollbarGrabActive  ] = m_Style->Colors.ScrollbarGrabActive;
		style.Colors[ImGuiCol_CheckMark			   ] = m_Style->Colors.CheckMark;
		style.Colors[ImGuiCol_SliderGrab		   ] = m_Style->Colors.SliderGrab;
		style.Colors[ImGuiCol_SliderGrabActive	   ] = m_Style->Colors.SliderGrabActive;
		style.Colors[ImGuiCol_Button			   ] = m_Style->Colors.Button;
		style.Colors[ImGuiCol_ButtonHovered		   ] = m_Style->Colors.ButtonHovered;
		style.Colors[ImGuiCol_ButtonActive		   ] = m_Style->Colors.ButtonActive;
		style.Colors[ImGuiCol_Header			   ] = m_Style->Colors.Header;
		style.Colors[ImGuiCol_HeaderHovered		   ] = m_Style->Colors.HeaderHovered;
		style.Colors[ImGuiCol_HeaderActive		   ] = m_Style->Colors.HeaderActive;
		style.Colors[ImGuiCol_Separator			   ] = m_Style->Colors.Separator;
		style.Colors[ImGuiCol_SeparatorHovered	   ] = m_Style->Colors.SeparatorHovered;
		style.Colors[ImGuiCol_SeparatorActive	   ] = m_Style->Colors.SeparatorActive;
		style.Colors[ImGuiCol_ResizeGrip		   ] = m_Style->Colors.ResizeGrip;
		style.Colors[ImGuiCol_ResizeGripHovered	   ] = m_Style->Colors.ResizeGripHovered;
		style.Colors[ImGuiCol_ResizeGripActive	   ] = m_Style->Colors.ResizeGripActive;
		style.Colors[ImGuiCol_Tab				   ] = m_Style->Colors.Tab;
		style.Colors[ImGuiCol_TabHovered		   ] = m_Style->Colors.TabHovered;
		style.Colors[ImGuiCol_TabActive			   ] = m_Style->Colors.TabActive;
		style.Colors[ImGuiCol_TabUnfocused		   ] = m_Style->Colors.TabUnfocused;
		style.Colors[ImGuiCol_TabUnfocusedActive   ] = m_Style->Colors.TabUnfocusedActive;
		style.Colors[ImGuiCol_DockingPreview	   ] = m_Style->Colors.DockingPreview;
		style.Colors[ImGuiCol_DockingEmptyBg	   ] = m_Style->Colors.DockingEmptyBg;
		style.Colors[ImGuiCol_PlotLines			   ] = m_Style->Colors.PlotLines;
		style.Colors[ImGuiCol_PlotLinesHovered	   ] = m_Style->Colors.PlotLinesHovered;
		style.Colors[ImGuiCol_PlotHistogram		   ] = m_Style->Colors.PlotHistogram;
		style.Colors[ImGuiCol_PlotHistogramHovered ] = m_Style->Colors.PlotHistogramHovered;
		style.Colors[ImGuiCol_TableHeaderBg		   ] = m_Style->Colors.TableHeaderBg;
		style.Colors[ImGuiCol_TableBorderStrong	   ] = m_Style->Colors.TableBorderStrong;
		style.Colors[ImGuiCol_TableBorderLight	   ] = m_Style->Colors.TableBorderLight;
		style.Colors[ImGuiCol_TableRowBg		   ] = m_Style->Colors.TableRowBg;
		style.Colors[ImGuiCol_TableRowBgAlt		   ] = m_Style->Colors.TableRowBgAlt;
		style.Colors[ImGuiCol_TextSelectedBg	   ] = m_Style->Colors.TextSelectedBg;
		style.Colors[ImGuiCol_DragDropTarget	   ] = m_Style->Colors.DragDropTarget;
		style.Colors[ImGuiCol_NavHighlight		   ] = m_Style->Colors.NavHighlight;
		style.Colors[ImGuiCol_NavWindowingHighlight] = m_Style->Colors.NavWindowingHighlight;
		style.Colors[ImGuiCol_NavWindowingDimBg	   ] = m_Style->Colors.NavWindowingDimBg;
		style.Colors[ImGuiCol_ModalWindowDimBg     ] = m_Style->Colors.ModalWindowDimBg;

		// Test vars later

		/*style.Alpha							= m_Style->Vars.Alpha;
		style.DisabledAlpha					= m_Style->Vars.DisabledAlpha;
		style.WindowPadding					= m_Style->Vars.WindowPadding;
		style.WindowRounding				= m_Style->Vars.WindowRounding;
		style.WindowBorderSize				= m_Style->Vars.WindowBorderSize;
		style.WindowMinSize					= m_Style->Vars.WindowMinSize;
		style.WindowTitleAlign				= m_Style->Vars.WindowTitleAlign;
		style.WindowMenuButtonPosition		= m_Style->Vars.WindowMenuButtonPosition;
		style.ChildRounding					= m_Style->Vars.ChildRounding;
		style.ChildBorderSize				= m_Style->Vars.ChildBorderSize;
		style.PopupRounding					= m_Style->Vars.PopupRounding;
		style.PopupBorderSize				= m_Style->Vars.PopupBorderSize;
		style.FramePadding					= m_Style->Vars.FramePadding;
		style.FrameRounding					= m_Style->Vars.FrameRounding;
		style.FrameBorderSize				= m_Style->Vars.FrameBorderSize;
		style.ItemSpacing					= m_Style->Vars.ItemSpacing;
		style.ItemInnerSpacing				= m_Style->Vars.ItemInnerSpacing;
		style.CellPadding					= m_Style->Vars.CellPadding;
		style.TouchExtraPadding				= m_Style->Vars.TouchExtraPadding;
		style.IndentSpacing					= m_Style->Vars.IndentSpacing;
		style.ColumnsMinSpacing				= m_Style->Vars.ColumnsMinSpacing;
		style.ScrollbarSize					= m_Style->Vars.ScrollbarSize;
		style.ScrollbarRounding				= m_Style->Vars.ScrollbarRounding;
		style.GrabMinSize					= m_Style->Vars.GrabMinSize;
		style.GrabRounding					= m_Style->Vars.GrabRounding;
		style.LogSliderDeadzone				= m_Style->Vars.LogSliderDeadzone;
		style.TabRounding					= m_Style->Vars.TabRounding;
		style.TabBorderSize					= m_Style->Vars.TabBorderSize;
		style.TabMinWidthForCloseButton		= m_Style->Vars.TabMinWidthForCloseButton;
		style.ColorButtonPosition			= m_Style->Vars.ColorButtonPosition;
		style.ButtonTextAlign				= m_Style->Vars.ButtonTextAlign;
		style.SelectableTextAlign			= m_Style->Vars.SelectableTextAlign;
		style.DisplayWindowPadding			= m_Style->Vars.DisplayWindowPadding;
		style.DisplaySafeAreaPadding		= m_Style->Vars.DisplaySafeAreaPadding;
		style.MouseCursorScale				= m_Style->Vars.MouseCursorScale;
		style.AntiAliasedLines				= m_Style->Vars.AntiAliasedLines;
		style.AntiAliasedLinesUseTex		= m_Style->Vars.AntiAliasedLinesUseTex;
		style.AntiAliasedFill				= m_Style->Vars.AntiAliasedFill;
		style.CurveTessellationTol			= m_Style->Vars.CurveTessellationTol;
		style.CircleTessellationMaxError	= m_Style->Vars.CircleTessellationMaxError;*/
	}
}
