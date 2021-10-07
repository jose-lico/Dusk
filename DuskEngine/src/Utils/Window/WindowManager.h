#pragma once

#include "Window.h"
#include "Core/Application/Macros.h"

namespace DuskEngine
{
	class DUSK_API WindowManager
	{
	public:
		static void Init();
		static void Shutdown();
		static Window* GetWindow() { return s_Window; }
	private:
		static Window* s_Window;
	};
}