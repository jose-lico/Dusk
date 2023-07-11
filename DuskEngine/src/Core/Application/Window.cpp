#include "pch.h"
#include "Window.h"

#include "Core.h"
#include "Core/Events/Events.h"
#include "Utils/Profiling/Timer.h"

#include "GLFW/glfw3.h"
#include "GL/glew.h"

#include <iostream>

namespace DuskEngine 
{
	extern void DropCallback(GLFWwindow* window, int count, const char** paths);

	Window::Window(const WindowData& data)
		:m_Data(data)
	{
		ScopeTimer windowTimer("Window creation");
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_SAMPLES, 4);
#if DUSK_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

		m_Window = glfwCreateWindow(data.Width, data.Height, data.Title.c_str(), NULL, NULL);

		if (!m_Window)
			ERR("Window wasn't created properly, shutting glfw down");

		TRACE("Window created");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		
		glfwSetWindowSizeLimits(m_Window, data.Width, data.Height, GLFW_DONT_CARE, GLFW_DONT_CARE);
		glfwSwapInterval(m_Data.VSync);

		// glfw callbacks
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32_t width, int32_t height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
				glViewport(0, 0, width, height); // maybe move to context
			});

		//glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximized)
		//	{
		//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		//		int width, height;
		//		glfwGetWindowSize(window, &width, &height);
		//		data.Width = width;
		//		data.Height = height;

		//		WindowResizeEvent event(width, height);
		//		data.EventCallback(event);
		//		glViewport(0, 0, width, height); // maybe move to context
		//		LOG("Maximized");
		//	});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});

		glfwSetDropCallback(m_Window, DropCallback);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}

	void Window::SetVSync(bool vsync)
	{
		m_Data.VSync = vsync;
		glfwSwapInterval(vsync);
	}

	void Window::SetWindowTitle(const std::string& title)
	{
		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	void Window::CenterWindow()
	{
		GLFWmonitor* primary = glfwGetPrimaryMonitor();

		const GLFWvidmode* mode = glfwGetVideoMode(primary);

		glfwSetWindowPos(m_Window, mode->width / 2 - m_Data.Width / 2, mode->height / 2 - m_Data.Height / 2);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}
}
