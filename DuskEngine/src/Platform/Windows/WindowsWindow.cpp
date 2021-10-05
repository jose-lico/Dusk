#include "pch.h"
#include "WindowsWindow.h"

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
			DUSK_LOG_ERROR("Window wasn't created properly, shutting glfw down");
			glfwTerminate();
		}

		DUSK_LOG_INFO("Window created");
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		DUSK_LOG_INFO("Window destroyed");
	}

	void WindowsWindow::SetVSync(bool vsync)
	{
		m_Data.VSync = vsync;
		glfwSwapInterval(vsync);
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