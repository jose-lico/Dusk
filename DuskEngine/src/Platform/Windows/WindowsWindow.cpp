#include "WindowsWindow.h"

namespace DuskEngine
{
	static bool HasSentDataToGPU = false;

	WindowsWindow::WindowsWindow(const WindowData& data)
	{
		Init(data);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowData& data)
	{
		m_Data = data;

		glfwInit();
		m_Window = glfwCreateWindow(data.Width, data.Height, "My Window", NULL, NULL);

		if (!m_Window)
		{
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(data.VSync);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
	void* WindowsWindow::GetNativeHandle() const
	{
		return m_Window;
	}
}