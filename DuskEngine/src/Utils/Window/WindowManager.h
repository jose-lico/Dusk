#pragma once

#include "Window.h"

namespace DuskEngine
{
	class WindowManager
	{
	public:
		static void Init();
		static void Shutdown();
		static Window* GetWindow() { return s_Window; }
	private:
		static Window* s_Window;
	};
}