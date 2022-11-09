#include "pch.h"
#include "OpenGLContext.h"

#include "GLCommon.h"

#include "GLFW/glfw3.h"

namespace DuskEngine
{
	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
		GLsizei length, const char* message, const void* userParam);

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		glfwMakeContextCurrent(m_WindowHandle);

		if (glewInit() != GLEW_OK)
			FATAL("Failed to initialize glew!");

#if DUSK_DEBUG
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			TRACE("OpenGL Debug Output enabled.");
		}
#endif

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

	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, 
		const char* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::string sourceMessage;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             sourceMessage = "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceMessage = "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceMessage = "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceMessage = "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     sourceMessage = "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           sourceMessage = "Source: Other"; break;
		}

		std::string typeMessage;
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               typeMessage = "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeMessage = "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeMessage = "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         typeMessage = "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         typeMessage = "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              typeMessage = "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          typeMessage = "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           typeMessage = "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               typeMessage = "Type: Other"; break;
		}

		std::string severityMessage;
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         severityMessage = "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       severityMessage = "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          severityMessage = "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severityMessage = "Severity: notification"; break;
		}

		std::string errorMessage = "Error code: " + std::to_string(id) + ": " + message + "\n"
			+ sourceMessage + "\n" + typeMessage + "\n" + severityMessage;

		ERR(errorMessage);
	}
}
