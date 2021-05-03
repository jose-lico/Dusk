#include "DuskEngine.h"

#include <iostream>
#include "GL/glew.h"

namespace DuskEngine
{
	DuskEngine::DuskEngine()
	{
	}

	int DuskEngine::Sum(int a, int b)
	{
		return a + b;
	}

	void DuskEngine::Greeting()
	{
		std::cout << "This is a message from the Dusk Engine DLL!" << std::endl;
	}

	Window::Window()
	{
		glfwInit();
		m_Window = glfwCreateWindow(1280, 720, "My Window", NULL, NULL);

		if (!m_Window)
		{
			glfwTerminate();
			std::cout << "Something failed" << std::endl;
		}

		glfwMakeContextCurrent(m_Window);

		std::cout << "Window Created" << std::endl;
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::Update()
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	bool Window::NotClose()
	{
		return glfwWindowShouldClose(m_Window);
	}
}