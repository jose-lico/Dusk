#include "pch.h"
#include "OpenGLContext.h"

#include "GLCommon.h"

#include "GLFW/glfw3.h"

namespace DuskEngine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		glfwMakeContextCurrent(m_WindowHandle);

		if (glewInit() != GLEW_OK)
			FATAL("Failed to initialize glew!");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		TRACE("OpenGL Context created and initialized.");
	}

	OpenGLContext::~OpenGLContext()
	{
		TRACE("OpenGL Context shutdown");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
		glfwPollEvents();
	}
}