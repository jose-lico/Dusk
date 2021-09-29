#include "pch.h"

#include "Application.h"
#include "Platform/Windows/WindowsWindow.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "gtc/type_ptr.hpp"


namespace DuskEngine
{
	Application::Application()
		:m_Camera(glm::perspective(glm::radians(45.0f), 16.0f/ 9.0f, 0.01f, 100.0f), glm::vec3(-1.0f, 0.0f, 3.0f), glm::vec3(0.0f, -90.0f, 0.0f))
	{
		WindowData data;
		m_Window = new WindowsWindow(data);

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
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(unsigned int)));

		VertexBufferLayout vbl;
		vbl.Push(ShaderDataType::Float, 3, true);
		vbl.Push(ShaderDataType::Float, 2, true);
		vertexBuffer->SetLayout(vbl);
		
		m_VA->Bind();
		m_VA->AddBuffer(vertexBuffer);
		m_VA->AddIndices(indexBuffer);

		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		m_VAFramebuffer.reset(VertexArray::Create());
		
		std::shared_ptr<VertexBuffer> vertexBufferFramebuffer;
		vertexBufferFramebuffer.reset(VertexBuffer::Create(quadVertices, sizeof(quadVertices)));

		VertexBufferLayout vblFramebuffer;
		vblFramebuffer.Push(ShaderDataType::Float, 2, true);
		vblFramebuffer.Push(ShaderDataType::Float, 2, true);

		m_VAFramebuffer->Bind();
		m_VAFramebuffer->AddBuffer(vertexBufferFramebuffer);
		vertexBufferFramebuffer->SetLayout(vblFramebuffer);

		m_Framebuffer = new Framebuffer(1280, 720);
		m_Framebuffer->Unbind();
	}

	Application::~Application()
	{
		glfwTerminate();
	}

	void Application::Run()
	{
		glEnable(GL_DEPTH_TEST);

		while(!m_Window->ShouldClose())
		{
			// TEMP
			//m_Framebuffer->Bind();
			//glEnable(GL_DEPTH_TEST);

			RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_ModelViewProjection", m_Camera.GetViewProjectionMatrix());
			m_Texture->Bind(0);
			Renderer::Submit(m_VA);

			Renderer::EndScene();

			// TEMP
			//m_Framebuffer->Unbind();
			//glDisable(GL_DEPTH_TEST);

			//RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
			//RenderCommand::Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//ImGui::Begin("Framebuffer");
			//ImGui::Image((void*)(intptr_t)m_Framebuffer->GetTexture(), ImVec2(1280, 720));
			//ImGui::End();

			//ImGui::ShowDemoWindow((bool*)true);

			ImGuiIO& io = ImGui::GetIO();
			int width;
			int height;
			glfwGetWindowSize((GLFWwindow*)m_Window->GetNativeHandle(), &width, &height);
			io.DisplaySize = ImVec2((float)width, (float)height);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			m_Window->OnUpdate();
		}
	}
}