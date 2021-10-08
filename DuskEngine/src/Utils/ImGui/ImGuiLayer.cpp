#include "pch.h"
#include "ImGuiLayer.h"
#include "Core/Macros/BIND_EVENT_FN.h"

#include "Utils/Window/WindowManager.h"

#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>
#include <imgui.h>

namespace DuskEngine
{
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

		//ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)WindowManager::GetWindow()->GetNativeHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
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
		if(e.GetKeyCode() == 32)
			m_ShowDemoWindow = true;

		return true;
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		int width;
		int height;
		glfwGetWindowSize((GLFWwindow*)WindowManager::GetWindow()->GetNativeHandle(), &width, &height);
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
}