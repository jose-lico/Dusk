#include "pch.h"

#include "OpenGLContext.h"

namespace DuskEngine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

	}

	void OpenGLContext::SetVSync(bool vsync)
	{
		glfwSwapInterval(vsync);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
		glfwPollEvents();
	}
}