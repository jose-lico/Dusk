#include "pch.h"

#include "OpenGLContext.h"
#include "GLCommon.h"

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
		if (glewInit() != GLEW_OK)
			std::cout << "Error" << std::endl;
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