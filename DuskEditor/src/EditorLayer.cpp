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

		delete model;
	}

	void EditorLayer::OnAttach()
	{
		// Soon to be gone :)
		{
			FramebufferSpecification fbSpec;
			fbSpec.Width = 720;
			fbSpec.Height = 480;
			m_FB.reset(FrameBuffer::Create(fbSpec));

			m_Scene = MakeRef<Scene>();

			Ref<Material> cubeMaterial = MakeRef<Material>(Shader::Create("res/shaders/phong.glsl"), "Phong Material");
			Ref<Material> quadMaterial = MakeRef<Material>(Shader::Create("res/shaders/simpleTexture.glsl"));
			Ref<Material> modelMaterial = MakeRef<Material>(Shader::Create("res/shaders/phong.glsl"), "Phong Material");
			Ref<Material> lightMaterial = MakeRef<Material>(Shader::Create("res/shaders/simpleColor.glsl"));

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

			auto light = m_Scene->CreateEntity("Point Light");
			auto& l = light.AddComponent<Light>();
			l.color = { 1.0f, 1.0f, 1.0f };
			l.type = LightType::Point;
			auto& lightTransform = light.GetComponent<Transform>();
			lightTransform.position = { -2.0f, 1.0f, 1.0f };
			lightTransform.scale = { 0.1f, 0.1f, 0.1f };
			light.AddComponent<MeshRenderer>(PrimitiveMesh::Cube(), lightMaterial);

			auto light2 = m_Scene->CreateEntity("Point Light 2");
			auto& l2 = light2.AddComponent<Light>();
			l2.color = { 1.0f, 0.0f, 0.0f };
			l2.type = LightType::Point;
			auto& lightTransform2 = light2.GetComponent<Transform>();
			lightTransform2.position = { 2.0f, 1.0f, 1.0f };
			lightTransform2.scale = { 0.1f, 0.1f, 0.1f };
			light2.AddComponent<MeshRenderer>(PrimitiveMesh::Cube(), lightMaterial);

			auto light3 = m_Scene->CreateEntity("Directional Light");
			auto& l3 = light3.AddComponent<Light>();
			l3.color = { 1.0f, 1.0f, 1.0f };
			l3.type = LightType::Directional;
			auto& lightTransform3 = light3.GetComponent<Transform>();
			lightTransform3.position = { -3.0f, 1.0f, 1.0f };
			lightTransform3.scale = { 0.1f, 0.1f, 0.1f };
			lightTransform3.rotation = glm::radians(glm::vec3(-15.0f, -40.0f, 0.0f));
			light3.AddComponent<MeshRenderer>(PrimitiveMesh::Cube(), lightMaterial);

			model = new Model("res/models/teapot.fbx");

			auto modelTeste = m_Scene->CreateEntity("Model");
			modelTeste.GetComponent<Transform>().position = { 3.0f, -.75f, 0.0f };
			modelTeste.AddComponent<MeshRenderer>(model->m_Meshes[0], modelMaterial);
		}

		m_Scene = MakeRef<Scene>();

		APP_LOG("End of scope")

		SceneSerializer::DeserializeText(m_Scene, "res/scenes/scene.yaml");
		camera = *m_Scene->GetMainCamera();

		m_Panels.push_back(new InspectorPanel());
		InspectorPanel& inspector = *(InspectorPanel*)m_Panels.back();
		m_Panels.push_back(new HierarchyPanel(m_Scene, inspector));
		m_Panels.push_back(new SceneViewportPanel(m_FB, camera));
		m_Panels.push_back(new ConsolePanel());
		m_Panels.push_back(new ContentBrowserPanel());
	}

	void EditorLayer::OnUpdate()
	{
		ResourceManager::CreateUUIDs();

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

		ImGui::Begin("Save Scene");

		if(ImGui::Button("Save Scene"))
			SceneSerializer::SerializeText(m_Scene, "res/scenes/scene.yaml");

		ImGui::End();

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