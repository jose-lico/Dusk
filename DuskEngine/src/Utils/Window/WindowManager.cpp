#include "pch.h"
#include "WindowManager.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Utils/Logging/Log.h"

namespace DuskEngine
{
	Window* WindowManager::s_Window;

	WindowManager::WindowManager()
	{
		DUSK_INFO("Window Manager constructor");
	}

	void WindowManager::Init()
	{
		DUSK_INFO("Window Manager initialized!");

		// For now both Linux and Windows will use WindowsWindow.h
#ifdef DUSK_WINDOWS
		s_Window = new WindowsWindow();
		s_Window->Init();
#elif DUSK_LINUX
		s_Window = new WindowsWindow();
		s_Window->Init();
#endif
	}

	void WindowManager::Shutdown()
	{
		s_Window->Shutdown();
	}
}