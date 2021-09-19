#pragma once

#include "Core/Renderer/RendererContext.h"

#include "GLFW/glfw3.h"

namespace DuskEngine
{
	class OpenGLContext : public RendererContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		virtual void Init() override;
		virtual void SetVSync(bool vsync) override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}