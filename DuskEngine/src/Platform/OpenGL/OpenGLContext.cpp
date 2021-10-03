#include "pch.h"

#include "OpenGLContext.h"
#include "GLCommon.h"
#include "Utils/Logging/Log.h"

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
			DUSK_ERROR("Failed to initialize glew");
	}

	void OpenGLContext::Shutdown()
	{
		glfwTerminate();
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