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
			DUSK_LOG_ERROR("Failed to initialize glew");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		DUSK_LOG_INFO("OpenGL Context created and initialized")
	}

	void OpenGLContext::Shutdown()
	{
		glfwTerminate();

		DUSK_LOG_INFO("OpenGL Context shutdown")
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
		glfwPollEvents();
	}
}