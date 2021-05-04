#pragma once

#include "defines.h"
#include "Window.h"
 
namespace DuskEngine
{
	class DUSK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		bool m_Running = true;
		Window* m_Window;
	};
}