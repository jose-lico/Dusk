#include "pch.h"
#include "ImGuiLayer.h"
#include "Style.h"

#include "Core/Events/Events.h"

#include "Core/Application/Window.h"

#include "IconsForkAwesome.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui/imgui.h"
#include "../backends/imgui_impl_opengl3.cpp"
#include "../backends/imgui_impl_glfw.cpp"

#include "ImGuizmo/ImGuizmo.h"

namespace DuskEngine
{
	Style* ImGuiLayer::m_Style;

	ImGuiLayer::ImGuiLayer(Window* window)
		:m_Window(window)
	{
		m_Style = new Style();
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 410");

		ApplyStyle();

		ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;

		//AddFont("res/editor/fonts/RobotoMono-Regular.ttf", 18.0f);
		AddFont("res/editor/fonts/Roboto-Regular.ttf", 18.0f);
		AddFont("res/editor/fonts/Roboto-Regular.ttf", 18.0f);
	}

	void ImGuiLayer::OnDetach()
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

	bool ImGuiLayer::ActivateDemoWindow(KeyPressedEvent& e)
	{
		if(e.GetKeyCode() == Key::P)
			m_ShowDemoWindow = true;

		return true;
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		int width;
		int height;
		glfwGetWindowSize((GLFWwindow*)m_Window->GetNativeHandle(), &width, &height);
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

	void ImGuiLayer::AddFont(const std::string& name, float size)
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromFileTTF(name.c_str(), size);

		static const ImWchar icons_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
		
		ImFontConfig icons_config;
		icons_config.PixelSnapH = true;
		icons_config.MergeMode = true;

#ifdef DUSK_WINDOWS
		io.Fonts->AddFontFromFileTTF("../Dependencies/Fork-Awesome/fonts/" FONT_ICON_FILE_NAME_FK, 18.0f, &icons_config, icons_ranges);
#elif DUSK_LINUX
		io.Fonts->AddFontFromFileTTF("../../../Dependencies/Fork-Awesome/fonts/" FONT_ICON_FILE_NAME_FK, 18.0f, &icons_config, icons_ranges);
#endif
	}

	void ImGuiLayer::ApplyStyle()
	{
		ImVec4 darkest(0.086f, 0.086f, 0.086f, 1.0f);
		ImVec4 dark(0.12f, 0.12f, 0.12f, 1.0f);
		ImVec4 grey1(0.15, 0.15, 0.15, 1.0f);
		ImVec4 grey2(0.2, 0.2, 0.2, 1.0f);
		ImVec4 grey3(0.25f, 0.25f, 0.25f, 1.0f);
		ImVec4 grey4(0.41f, 0.5f, 0.52f, 1.0f);
		ImVec4 white(0.87f, 0.87f, 0.87f, 1.0f);
		
		ImVec4 red(1.0f, 0.0f, 0.0f, 1.0f);

		ImGuiStyle& style = ImGui::GetStyle();

		style.TabRounding = 0.0f;
		style.FramePadding = { 5.0f, 4 };
		//style.TabBorderSize = 10.0f;
		//TRACE(std::to_string(style.TabBorderSize).c_str());
		
		style.Colors[ImGuiCol_Text] = m_Style->Colors.ImGuiCol_Text;
		style.Colors[ImGuiCol_TextDisabled] = grey3;

		style.Colors[ImGuiCol_MenuBarBg] = dark;
		style.Colors[ImGuiCol_Border] = dark;
		style.Colors[ImGuiCol_Separator] = dark;
		style.Colors[ImGuiCol_SeparatorHovered] = dark;
		style.Colors[ImGuiCol_SeparatorActive] = dark;
		
		style.Colors[ImGuiCol_WindowBg] = grey2;

		style.Colors[ImGuiCol_FrameBg] = grey1;
		style.Colors[ImGuiCol_TitleBg] = dark;
		style.Colors[ImGuiCol_TitleBgActive] = dark;
		style.Colors[ImGuiCol_TitleBgCollapsed] = dark;
		style.Colors[ImGuiCol_Header] = dark;

		style.Colors[ImGuiCol_Tab] = grey1;
		style.Colors[ImGuiCol_TabHovered] = grey3;
		style.Colors[ImGuiCol_TabActive] = grey3;
		style.Colors[ImGuiCol_TabUnfocused] = grey1;
		style.Colors[ImGuiCol_TabUnfocusedActive] = grey2;

		style.Colors[ImGuiCol_HeaderHovered] = dark;
		style.Colors[ImGuiCol_PopupBg] = darkest;

		style.Colors[ImGuiCol_BorderShadow] = dark;

		//style.Colors[ImGuiCol_Header] = dark;
		//style.Colors[ImGuiCol_NavHighlight] = dark;
		
		/*style.Colors[ImGuiCol_ChildBg] = dark;
		style.Colors[ImGuiCol_PopupBg] = dark;
		

		style.Colors[ImGuiCol_FrameBgHovered] = dark;
		style.Colors[ImGuiCol_FrameBgActive] = dark;
		
		
		
		style.Colors[ImGuiCol_ScrollbarBg] = dark;
		style.Colors[ImGuiCol_ScrollbarGrab] = dark;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = dark;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = dark;
		style.Colors[ImGuiCol_CheckMark] = dark;
		style.Colors[ImGuiCol_SliderGrab] = dark;
		style.Colors[ImGuiCol_SliderGrabActive] = dark;
		style.Colors[ImGuiCol_Button] = grey3;
		style.Colors[ImGuiCol_ButtonHovered] = grey3;
		style.Colors[ImGuiCol_ButtonActive] = grey3;
		
		style.Colors[ImGuiCol_HeaderHovered] = dark;
		style.Colors[ImGuiCol_HeaderActive] = dark;
		
		style.Colors[ImGuiCol_ResizeGrip] = dark;
		style.Colors[ImGuiCol_ResizeGripHovered] = dark;
		style.Colors[ImGuiCol_ResizeGripActive] = dark;

		style.Colors[ImGuiCol_DockingPreview] = dark;
		style.Colors[ImGuiCol_DockingEmptyBg] = dark;
		style.Colors[ImGuiCol_PlotLines] = dark;
		style.Colors[ImGuiCol_PlotLinesHovered] = dark;
		style.Colors[ImGuiCol_PlotHistogram] = dark;
		style.Colors[ImGuiCol_PlotHistogramHovered] = dark;
		style.Colors[ImGuiCol_TextSelectedBg] = dark;
		style.Colors[ImGuiCol_DragDropTarget] = dark;
		style.Colors[ImGuiCol_NavHighlight] = dark;
		style.Colors[ImGuiCol_NavWindowingHighlight] = dark;
		style.Colors[ImGuiCol_NavWindowingDimBg] = dark;
		style.Colors[ImGuiCol_ModalWindowDimBg] = dark;*/
	}
}