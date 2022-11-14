#include "LauncherLayer.h"

#include "DuskEditor.h"

#include "Core/Application/Core.h"
#include "Core/Application/Window.h"
#include "Platform/OpenGL/OpenGLAPI.h"

#include "imgui/imgui.h"

namespace DuskEngine
{
	LauncherLayer::LauncherLayer(DuskEditor* editor)
		:m_Editor(editor)
	{
		m_Logger = new Logger(LOGGER);

		auto& app = Application::Get();

		WindowData data;
		Window& window = app.CreateWindow(data);
		OpenGLAPI::CreateContext(window.GetNativeHandle());
		app.SetImGuiGLContext();
		window.CenterWindow();
	}

	LauncherLayer::~LauncherLayer()
	{
		delete m_Logger;
	}

	void LauncherLayer::OnUpdate()
	{
		if(m_LaunchEditor)
		{
			m_Editor->LaunchEditor();
			TRACE("Launched");
		}
	}

	void LauncherLayer::OnImGuiRender()
	{
		m_Dockspace.BeginDockspace();

		ImGui::Begin("Launcher");
		
		ImGui::Text("This is the launcher");
		
		if (ImGui::Button("Launch"))
			m_LaunchEditor = true;

		ImGui::End();
		m_Dockspace.EndDockspace();
	}
}
