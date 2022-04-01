#pragma once
#include "Core/Renderer/RendererContext.h"

struct GLFWwindow;

namespace DuskEngine
{
	class OpenGLContext : public RendererContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}