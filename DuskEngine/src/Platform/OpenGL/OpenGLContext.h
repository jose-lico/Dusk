#pragma once

 struct GLFWwindow;

namespace DuskEngine
{
	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		void SwapBuffers();
	private:
		GLFWwindow* m_WindowHandle;
	};
}