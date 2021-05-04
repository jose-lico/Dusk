#include "WindowsWindow.h"

namespace DuskEngine
{
	static bool HasSentDataToGPU = false;

	WindowsWindow::WindowsWindow(const WindowData& data)
	{
		Init(data);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();

		delete m_VA;
		delete m_VB;
		delete m_IB;
	}

	void WindowsWindow::Init(const WindowData& data)
	{
		m_Data = data;

		glfwInit();
		m_Window = glfwCreateWindow(data.Width, data.Height, "My Window", NULL, NULL);

		if (!m_Window)
		{
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(data.VSync);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		// TEMP
		if(!HasSentDataToGPU)
		{
			HasSentDataToGPU = true;

			float vertices[] = {
				// first triangle
				 0.5f,  0.5f, 0.0f,  // top right
				 0.5f, -0.5f, 0.0f,  // bottom right
				-0.5f,  0.5f, 0.0f,  // top left 
				// second triangle
				 0.5f, -0.5f, 0.0f,  // bottom right
				-0.5f, -0.5f, 0.0f,  // bottom left
				-0.5f,  0.5f, 0.0f   // top left
			};

			unsigned int indices[] = {  // note that we start from 0!
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};

			m_VA = new VertexArray;
			m_VB = new VertexBuffer(sizeof(vertices), vertices);
			m_IB = new IndexBuffer(6, indices);

			VertexBufferLayout vbl;
			vbl.Push<float>(3);

			m_VA->Bind();
			m_VA->AddBuffer(*m_VB);
			m_VA->AddIndices(*m_IB);
			m_VA->AddLayout(vbl);
		}

		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TEMP
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
	void* WindowsWindow::GetNativeHandle() const
	{
		return m_Window;
	}
}