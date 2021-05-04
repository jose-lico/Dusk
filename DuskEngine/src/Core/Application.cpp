#include "Application.h"
#include "Platform/Windows/WindowsWindow.h"

#include <iostream>

namespace DuskEngine
{
	Application::Application()
	{
		WindowData data;
		m_Window = new WindowsWindow(data);

		if (glewInit() != GLEW_OK)
			std::cout << "Error" << std::endl;
	}

	Application::~Application()
	{
		glfwTerminate();
	}

	void Application::Run()
	{
		while(m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}