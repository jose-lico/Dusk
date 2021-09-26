#include "pch.h"

#include "Application.h"
#include "Platform/Windows/WindowsWindow.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace DuskEngine
{
	Application::Application()
	{
		WindowData data;
		m_Window = new WindowsWindow(data);

		m_Renderer.Init();

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

		m_Texture = std::make_shared<Texture>("res/textures/rocks.png", GL_RGBA);
		
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
	}

	Application::~Application()
	{
		glfwTerminate();
	}

	void Application::Run()
	{
		while(!m_Window->ShouldClose())
		{
			m_Framebuffer->Bind();
			glEnable(GL_DEPTH_TEST);

			m_Renderer.ClearColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			m_Renderer.Clear();

			// TEMP
			m_Texture->Bind(0);
			m_Renderer.DrawElements(m_VA, m_Shader);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);

			m_Renderer.ClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			m_Renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Framebuffer");
			ImGui::Image((void*)(intptr_t)m_Framebuffer->GetTexture(), ImVec2(1280, 720));
			ImGui::End();

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