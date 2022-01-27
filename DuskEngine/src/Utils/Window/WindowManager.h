#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Window.h"

namespace DuskEngine
{
	class DUSK_EXPORT WindowManager
	{
	public:
		WindowManager();
		~WindowManager();
		
		Window* GetWindow() { return m_Window; }
	private:
		Window* m_Window;
	};
}