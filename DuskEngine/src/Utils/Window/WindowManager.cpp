#include "pch.h"
#include "WindowManager.h"

#include "Platform/Windows/WindowsWindow.h"

#include "GLFW/glfw3.h"

namespace DuskEngine
{
	WindowManager::WindowManager()
	{
		TRACE("Here");
		glfwInit();

		// For now both Linux and Windows will use WindowsWindow.h
#ifdef  DUSK_WINDOWS
		m_Window = new WindowsWindow();
#elif	DUSK_LINUX
		m_Window = new WindowsWindow();
#endif
	}

	WindowManager::~WindowManager()
	{
		delete m_Window;

		glfwTerminate();
	}
}