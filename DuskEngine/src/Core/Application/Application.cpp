#include "pch.h"
#include "Application.h"

#include "Utils/Logging/Logger.h"
#include "Utils/Window/WindowManager.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderCommand.h"

#include "gtc/type_ptr.hpp"
#include "Utils/Logging/Log.h"

#include "imgui.h"

namespace DuskEngine
{
	Application::Application()
	{
		Init();
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Init()
	{
		// Initialize subsystems
		Logger::Init();
		WindowManager::Init();
		WindowManager::GetWindow()->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		rendererContext = RendererContext::Create(WindowManager::GetWindow()->GetNativeHandle());
		rendererContext->Init();
		Renderer::Init();
		
		m_ImGuiLayer = new ImGuiLayer();
		m_LayerStack.PushOverlay(m_ImGuiLayer);
		m_ImGuiLayer->OnAttach();

		{
			m_Camera = new Camera(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f), glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, -90.0f, 0.0f));

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
	}

	void Application::OnEvent(Event& e)
	{
		DUSK_LOG_DEBUG(e.ToString());
	}
	
	void Application::Run()
	{
		static bool showDemoWindow = true;
		static bool* showDemoWindowPtr;
		showDemoWindowPtr = &showDemoWindow;

		while (!WindowManager::GetWindow()->ShouldClose())
		{
			RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
			RenderCommand::Clear();

			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_ModelViewProjection", m_Camera->GetViewProjectionMatrix());
			m_Texture->Bind(0);
			Renderer::Submit(m_VA);

			m_ImGuiLayer->Begin();
			if(showDemoWindow) ImGui::ShowDemoWindow(showDemoWindowPtr);
			m_ImGuiLayer->End();

			rendererContext->SwapBuffers();
		}
	}

	void Application::Shutdown()
	{
		DUSK_LOG_INFO("##### SHUTDOWN #####");

		// Shutdown subsystems
		Renderer::Shutdown();
		rendererContext->Shutdown();
		WindowManager::Shutdown();
		Logger::Shutdown();
	}
}