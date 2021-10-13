#include "pch.h"
#include "OpenGLContext.h"
#include "GLCommon.h"

#include "Core/Macros/LOG.h"

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
			FATAL("Failed to initialize glew");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		TRACE("OpenGL Context created and initialized")
	}

	void OpenGLContext::Shutdown()
	{
		glfwTerminate();

		TRACE("OpenGL Context shutdown")
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
		glfwPollEvents();
	}
}