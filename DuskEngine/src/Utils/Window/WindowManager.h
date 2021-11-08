#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Window.h"

namespace DuskEngine
{
	class DUSK_EXPORT WindowManager
	{
	public:
		static void Init();
		static void Shutdown();
		static Window* GetWindow() { return s_Window; }
	private:
		static Window* s_Window;
	};
}