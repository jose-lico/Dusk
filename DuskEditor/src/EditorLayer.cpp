#include "EditorLayer.h"
#include "DuskEngine.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

EditorLayer::EditorLayer()
{
	m_Camera = new DuskEngine::Camera(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f),\
		glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, -90.0f, 0.0f));
}

void EditorLayer::OnAttach()
{
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

	DuskEngine::FramebufferSpecification fbSpec;
	fbSpec.Width = 720;
	fbSpec.Height = 480;
	m_FB.reset(DuskEngine::FrameBuffer::Create(fbSpec));

	scenePanel = new ScenePanel(m_FB, *m_Camera);

	m_Shader.reset(DuskEngine::Shader::Create("res/shaders/simpleTexture.glsl"));

	m_Texture = std::make_shared<DuskEngine::Texture>("res/textures/uv_mapper.jpg", DuskEngine::Texture::TextureType::RGB);

	m_VA.reset(DuskEngine::VertexArray::Create());

	std::shared_ptr<DuskEngine::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(DuskEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

	std::shared_ptr<DuskEngine::IndexBuffer> indexBuffer;
	indexBuffer.reset(DuskEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

	std::shared_ptr<DuskEngine::VertexBufferLayout> vbl;
	vbl = std::make_shared<DuskEngine::VertexBufferLayout>();
	vbl->Push(DuskEngine::ShaderDataType::Float, 3, true);
	vbl->Push(DuskEngine::ShaderDataType::Float, 2, true);
	vertexBuffer->SetLayout(vbl);

	m_VA->Bind();
	m_VA->AddBuffer(vertexBuffer);
	m_VA->AddIndices(indexBuffer);

	m_Scene = std::make_shared<DuskEngine::Scene>();

	m_Entity = m_Scene->CreateEntity();
	m_Entity.AddComponent<DuskEngine::TransformC>();
}

void EditorLayer::OnDetach()
{
	m_Entity.DestroyEntity();
}

void EditorLayer::OnUpdate()
{
	m_Scene->OnUpdate();

	m_FB->Bind();
	DuskEngine::RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
	DuskEngine::RenderCommand::Clear();

	// dodgy camera controller
	{
		glm::vec3 pos = m_Camera->GetPosition();
		glm::vec3 input(0.0f);

		if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::D))
			input.x += 3.0f * DuskEngine::Time::GetDeltaTime();
		else if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::A))
			input.x -= 3.0f * DuskEngine::Time::GetDeltaTime();

		if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::W))
			input.z -= 3.0f * DuskEngine::Time::GetDeltaTime();
		else if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::S))
			input.z += 3.0f * DuskEngine::Time::GetDeltaTime();

		if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::Q))
			input.y += 3.0f * DuskEngine::Time::GetDeltaTime();
		else if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::E))
			input.y -= 3.0f * DuskEngine::Time::GetDeltaTime();

		pos += input;
		m_Camera->SetPosition(pos);

		glm::vec3 rot = m_Camera->GetRotation();

		if (!movingCamera && DuskEngine::Input::IsMouseButtonPressed(DuskEngine::Mouse::MOUSE_BUTTON_2))
		{
			DuskEngine::Input::SetCursorActive(DuskEngine::Cursor::CURSOR_DISABLED);
			movingCamera = true;
			firstMouse = true;
		}
		else if (!DuskEngine::Input::IsMouseButtonPressed(DuskEngine::Mouse::MOUSE_BUTTON_2))
		{
			DuskEngine::Input::SetCursorActive(DuskEngine::Cursor::CURSOR_NORMAL);
			movingCamera = false;
		}

		if (movingCamera)
		{
			if (firstMouse)
			{
				lastX = DuskEngine::Input::GetMouseX();
				lastY = DuskEngine::Input::GetMouseY();
				firstMouse = false;
			}

			float xoffset = DuskEngine::Input::GetMouseX() - lastX;
			float yoffset = lastY - DuskEngine::Input::GetMouseY(); // reversed since y-coordinates go from bottom to top

			lastX = DuskEngine::Input::GetMouseX();
			lastY = DuskEngine::Input::GetMouseY();

			xoffset *= 20.0f * DuskEngine::Time::GetDeltaTime();
			yoffset *= 20.0f * DuskEngine::Time::GetDeltaTime();

			rot.x += yoffset;
			rot.y += xoffset;
			m_Camera->SetRotation(rot);
		}
	}

	m_Shader->Bind();
	//m_Shader->SetUniformVec3("u_Color", m_Entity.GetComponent<DuskEngine::MeshRenderer>().Color);
	m_Shader->SetUniformVec3("u_Color", glm::vec3{1.0f});
	m_Shader->SetUniformMat4("u_ModelViewProjection", m_Camera->GetViewProjectionMatrix());
	m_Texture->Bind(0);
	DuskEngine::Renderer::Submit(m_VA);
	m_FB->Unbind();

	DuskEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	DuskEngine::RenderCommand::Clear();
}

void EditorLayer::OnImGuiRender()
{
	static bool p_open = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				*(&p_open) = false;
			ImGui::EndMenu();
		}

		scenePanel->OnImGuiRender();
		static bool demo = true;
		ImGui::ShowDemoWindow((bool*)demo);

		ImGui::EndMenuBar();
	}

	ImGui::End();
}
