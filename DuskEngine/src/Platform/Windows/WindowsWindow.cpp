#include "pch.h"

#include "WindowsWindow.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Utils/Logging/Log.h"

namespace DuskEngine
{
	void WindowsWindow::Init(const WindowData& data)
	{
		m_Data = data;

		glfwInit();
		m_Window = glfwCreateWindow(data.Width, data.Height, data.Title.c_str(), NULL, NULL);

		if (!m_Window)
		{
			DUSK_LOG("Window wasn't created properly, shutting glfw down");
			glfwTerminate();
		}
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate(RendererContext* context)
	{
		context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool vsync)
	{
		//m_Context->SetVSync(vsync);
		//m_Data.VSync = vsync;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void* WindowsWindow::GetNativeHandle() const
	{
		return m_Window;
	}
}