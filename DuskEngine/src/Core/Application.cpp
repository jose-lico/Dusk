#include "Application.h"
#include "Platform/Windows/WindowsWindow.h"

#include <iostream>

namespace DuskEngine
{
	Application::Application()
	{
		WindowData data;
		m_Window = new WindowsWindow(data);

		if (glewInit() != GLEW_OK)
			std::cout << "Error" << std::endl;

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

		m_Shader = new Shader("C:/Documents/Dusk/DuskEngine/res/shaders/simple.glsl");
		m_VA = new VertexArray;
		m_VB = new VertexBuffer(sizeof(vertices), vertices);
		m_IB = new IndexBuffer(6, indices);

		VertexBufferLayout vbl;
		vbl.Push<float>(3);

		m_VA->Bind();
		m_VA->AddBuffer(*m_VB);
		m_VA->AddIndices(*m_IB);
		m_VA->AddLayout(vbl);
		m_Shader->Bind();
	}

	Application::~Application()
	{
		delete m_VB;
		delete m_IB;
		delete m_VA;

		glfwTerminate();
	}

	void Application::Run()
	{
		while(m_Running)
		{
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// TEMP
			glDrawArrays(GL_TRIANGLES, 0, 6);

			m_Window->OnUpdate();
		}
	}
}