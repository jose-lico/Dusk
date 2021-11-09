#include "EditorLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace DuskEngine
{
	EditorLayer::EditorLayer()
	{
		CREATE_APP_LOGGER
		APP_TRACE("Tracing from the app")
		APP_LOG("Logging from the app")
	}

	EditorLayer::~EditorLayer()
	{
		for (Panel* panel : m_Panels)
		{
			delete panel;
		}
	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification fbSpec;
		fbSpec.Width = 720;
		fbSpec.Height = 480;
		m_FB.reset(FrameBuffer::Create(fbSpec));

		m_Scene = MakeRef<Scene>();

		Ref<Material> cubeMaterial = MakeRef<Material>(Shader::Create("res/shaders/phong.glsl"), "Phong-Material");
		Ref<Material> quadMaterial = MakeRef<Material>(Shader::Create("res/shaders/simpleTexture.glsl"));
		cubeMaterial->SetTexture("Diffuse", Texture::Create("res/textures/diffuse.png"));
		cubeMaterial->SetTexture("Specular", Texture::Create("res/textures/specular.png"));
		quadMaterial->SetTexture("Texture", Texture::Create("res/textures/uv_mapper.jpg"));

		auto quad = m_Scene->CreateEntity("Unlit Quad");
		quad.AddComponent<MeshRenderer>(PrimitiveMesh::Quad(), quadMaterial);
		
		auto cube = m_Scene->CreateEntity("Lit Cube");
		cube.GetComponent<Transform>().position = { -2.0f, 0.0f, 0.0f };
		cube.AddComponent<MeshRenderer>(PrimitiveMesh::Cube(), cubeMaterial);
		
		camera = m_Scene->CreateEntity("Editor Camera");
		camera.AddComponent<Camera>().projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f);
		auto& cameraTransform = camera.GetComponent<Transform>();
		cameraTransform.position = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraTransform.rotation = glm::radians(glm::vec3(0.0f, -90.0f, 0.0f));
		
		auto light = m_Scene->CreateEntity("Directional Light");
		light.AddComponent<Light>().color = {1.0f, 1.0f, 1.0f};
		auto& lightTransform = light.GetComponent<Transform>();
		lightTransform.position = { -2.0f, 1.0f, 1.0f };
		lightTransform.rotation = glm::radians(glm::vec3(-15.0f,-40.0f, 0.0f));

		m_Panels.push_back(new InspectorPanel());
		InspectorPanel& inspector = *(InspectorPanel*)m_Panels.back();
		m_Panels.push_back(new HierarchyPanel(m_Scene, inspector));
		m_Panels.push_back(new SceneViewportPanel(m_FB, camera));
		m_Panels.push_back(new ConsolePanel());
	}

	void EditorLayer::OnUpdate()
	{
		m_FB->Bind();

		EditorCamera();

		m_Scene->OnUpdate();

		m_FB->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		m_Dockspace.BeginDockspace();

		for (Panel* panel : m_Panels)
		{
			panel->OnImGuiRender();
		}

		m_Dockspace.EndDockspace();
	}

	// rework in the future to normalize input yadda yadda ya

	void EditorLayer::EditorCamera()
	{
		auto& transform = camera.GetComponent<Transform>();
		glm::vec3 pos = transform.position;
		
		float velocity = 3.0f * Time::GetDeltaTime();

		if (Input::IsKeyPressed(Key::W))
			pos += transform.front * velocity;
		else if (Input::IsKeyPressed(Key::S))
			pos -= transform.front * velocity;

		if (Input::IsKeyPressed(Key::D))
			pos += transform.right * velocity;
		else if (Input::IsKeyPressed(Key::A))
			pos -= transform.right * velocity;

		if (Input::IsKeyPressed(Key::E))
			pos.y += velocity;
		else if (Input::IsKeyPressed(Key::Q))
			pos.y -= velocity;

		transform.position = pos;

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
			glm::vec3 rot = transform.rotation;

			if (firstMouse)
			{
				lastX = Input::GetMouseX();
				lastY = Input::GetMouseY();
				firstMouse = false;
			}

			float xoffset = Input::GetMouseX() - lastX;
			float yoffset = lastY - Input::GetMouseY();

			lastX = Input::GetMouseX();
			lastY = Input::GetMouseY();

			xoffset *= 0.5f * Time::GetDeltaTime();
			yoffset *= 0.5f* Time::GetDeltaTime();

			rot.x += yoffset;
			rot.y += xoffset;
			transform.rotation = rot;
		}
	}
}