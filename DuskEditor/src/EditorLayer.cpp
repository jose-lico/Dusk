#include "EditorLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace DuskEngine
{
	EditorLayer::EditorLayer()
	{
		CREATE_APP_LOGGER
		APP_TRACE("Tracing from the app")
		APP_LOG("Logging from the app")
	}

	void EditorLayer::OnAttach()
	{
		{
			m_Camera = std::make_shared<Camera>(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f), \
				glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, -90.0f, 0.0f));

			float vertices[] =
			{
				// positions        // tex		// normals
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top right
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
				-0.5f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
			};

			unsigned int indices[] =
			{
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};

			FramebufferSpecification fbSpec;
			fbSpec.Width = 720;
			fbSpec.Height = 480;
			m_FB.reset(FrameBuffer::Create(fbSpec));

			m_Shader.reset(Shader::Create("res/shaders/simpleTexture.glsl"));
			m_Texture.reset(Texture::Create("res/textures/uv_mapper.jpg"));
			m_VA.reset(VertexArray::Create());

			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

			std::shared_ptr<VertexBufferLayout> vbl;
			vbl = std::make_shared<VertexBufferLayout>();
			vbl->Push(ShaderDataType::Float, 3, true);
			vbl->Push(ShaderDataType::Float, 2, true);
			vbl->Push(ShaderDataType::Float, 3, true);
			vertexBuffer->SetLayout(vbl);

			m_VA->Bind();
			m_VA->AddBuffer(vertexBuffer);
			m_VA->AddIndices(indexBuffer);
		}

		m_SceneEntt = std::make_shared<SceneEntt>(m_Camera);
		auto ent = m_SceneEntt->CreateEntity();
		auto ent2 = m_SceneEntt->CreateEntity();

		ent.AddComponent<Transform>();
		auto& t = ent2.AddComponent<Transform>();
		t.Position = glm::vec3(-2.0f, 0.0f, 0.0f);
		auto& mesh = ent.AddComponent<MeshRenderer>();
		auto& mesh2 = ent2.AddComponent<MeshRenderer>();

		mesh.VA = m_VA;
		mesh.SH = m_Shader;
		mesh.TX = m_Texture;

		mesh2.VA = m_VA;
		mesh2.SH = m_Shader;
		mesh2.TX = m_Texture;

		inspector = new InspectorPanel();
		m_Panels.push_back(inspector);
		m_Panels.push_back(new HierarchyPanel(m_SceneEntt, inspector));
		m_Panels.push_back(new SceneViewportPanel(m_FB, m_Camera));
	}

	void EditorLayer::OnUpdate()
	{
		m_FB->Bind();
		RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1 });
		RenderCommand::Clear();

		// dodgy camera controller
		{
			glm::vec3 pos = m_Camera->GetPosition();
			glm::vec3 input(0.0f);

			if (Input::IsKeyPressed(Key::D))
				input.x += 3.0f * Time::GetDeltaTime();
			else if (Input::IsKeyPressed(Key::A))
				input.x -= 3.0f * Time::GetDeltaTime();

			if (Input::IsKeyPressed(Key::W))
				input.z -= 3.0f * Time::GetDeltaTime();
			else if (Input::IsKeyPressed(Key::S))
				input.z += 3.0f * Time::GetDeltaTime();

			if (Input::IsKeyPressed(Key::Q))
				input.y += 3.0f * Time::GetDeltaTime();
			else if (Input::IsKeyPressed(Key::E))
				input.y -= 3.0f * Time::GetDeltaTime();

			pos += input;
			m_Camera->SetPosition(pos);

			glm::vec3 rot = m_Camera->GetRotation();

			if (!movingCamera && Input::IsMouseButtonPressed(Mouse::MOUSE_BUTTON_2))
			{
				Input::SetCursorActive(Cursor::CURSOR_DISABLED);
				movingCamera = true;
				firstMouse = true;
			}
			else if (!Input::IsMouseButtonPressed(Mouse::MOUSE_BUTTON_2))
			{
				Input::SetCursorActive(Cursor::CURSOR_NORMAL);
				movingCamera = false;
			}

			if (movingCamera)
			{
				if (firstMouse)
				{
					lastX = Input::GetMouseX();
					lastY = Input::GetMouseY();
					firstMouse = false;
				}

				float xoffset = Input::GetMouseX() - lastX;
				float yoffset = lastY - Input::GetMouseY(); // reversed since y-coordinates go from bottom to top

				lastX = Input::GetMouseX();
				lastY = Input::GetMouseY();

				xoffset *= 20.0f * Time::GetDeltaTime();
				yoffset *= 20.0f * Time::GetDeltaTime();

				rot.x += yoffset;
				rot.y += xoffset;
				m_Camera->SetRotation(rot);
			}
		}

		m_SceneEntt->OnUpdate();

		m_FB->Unbind();

		//RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		//RenderCommand::Clear();
	}

	void EditorLayer::OnImGuiRender()
	{
		m_Dockspace.BeginDockspace();

		for (PanelBase* panel : m_Panels)
		{
			panel->OnImGuiRender();
		}

		m_Dockspace.EndDockspace();
	}
}