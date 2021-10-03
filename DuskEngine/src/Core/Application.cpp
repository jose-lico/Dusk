#include "pch.h"

#include "Application.h"
#include "GL/glew.h"
#include "Platform/Windows/WindowsWindow.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "gtc/type_ptr.hpp"
#include "Utils/Logging/Log.h"


namespace DuskEngine
{
	Application::Application()
	{
		Init();
	}

	void Application::Init()
	{
		// Initialize subsystems
		logger->Init();
		windowManager->Init();
		contextManager = RendererContext::Create(windowManager->GetWindow()->GetNativeHandle());
		contextManager->Init();
		renderer->Init();
		
		m_Camera = new Camera(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f), glm::vec3(-1.0f, 0.0f, 3.0f), glm::vec3(0.0f, -90.0f, 0.0f));
		
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
		
		m_Shader = std::make_shared<Shader>("res/shaders/simpleTexture.glsl");

		m_Texture = std::make_shared<Texture>("res/textures/uv_mapper.jpg", GL_RGB);

		m_VA.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

		VertexBufferLayout vbl;
		vbl.Push(ShaderDataType::Float, 3, true);
		vbl.Push(ShaderDataType::Float, 2, true);
		vertexBuffer->SetLayout(vbl);

		m_VA->Bind();
		m_VA->AddBuffer(vertexBuffer);
		m_VA->AddIndices(indexBuffer);
	}

	void Application::Run()
	{
		while (!windowManager->GetWindow()->ShouldClose())
		{
			RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
			RenderCommand::Clear();

			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_ModelViewProjection", m_Camera->GetViewProjectionMatrix());
			m_Texture->Bind(0);
			Renderer::Submit(m_VA);

			contextManager->SwapBuffers();
		}
	}

	void Application::Shutdown()
	{
		// Shutdown subsystems
		renderer->Shutdown();
		contextManager->Shutdown();
		windowManager->Shutdown();
		logger->Shutdown();
	}
}