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

				 
		float vertices[] = 
		{
			// positions        // texture coords
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f  // top left 
		};

		unsigned int indices[] = 
		{
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		m_Shader = new Shader("res/shaders/simpleTexture.glsl");
		m_Texture = new Texture("res/textures/rocks.png", GL_RGBA);
		m_VA = new VertexArray;
		m_VB = new VertexBuffer(sizeof(vertices), vertices);
		m_IB = new IndexBuffer(6, indices);

		VertexBufferLayout vbl;
		vbl.Push<float>(3);
		vbl.Push<float>(2);

		m_VA->Bind();
		m_VA->AddBuffer(*m_VB);
		m_VA->AddIndices(*m_IB);
		m_VA->AddLayout(vbl);
		m_Texture->Bind(0);
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
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			m_Window->OnUpdate();
		}
	}
}