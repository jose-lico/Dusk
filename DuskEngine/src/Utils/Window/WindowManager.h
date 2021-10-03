#pragma once

#include "Window.h"

namespace DuskEngine
{
	class WindowManager
	{
	public:
		WindowManager();
		~WindowManager() = default;
		void Init();
		void Shutdown();
		Window* GetWindow() { return s_Window; }
	private:
		static Window* s_Window;
	};
}