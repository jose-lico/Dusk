#pragma once

#include "defines.h"

#include "GLFW/glfw3.h"

namespace DuskEngine
{
	class DUSK_API DuskEngine
	{
	public:
		DuskEngine();

		int Sum(int a, int b);
		void Greeting();
	};

	class DUSK_API Window
	{
	public:
		Window();
		~Window();
		void Update();
		bool NotClose();
	private:
		GLFWwindow* m_Window;
	};
}