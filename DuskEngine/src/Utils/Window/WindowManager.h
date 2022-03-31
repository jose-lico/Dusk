#pragma once
#include "Core/Macros/DUSK_API.h"

namespace DuskEngine
{
	class Window;

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