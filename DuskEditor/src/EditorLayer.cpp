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

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification fbSpec;
		fbSpec.Width = 720;
		fbSpec.Height = 480;
		m_FB.reset(FrameBuffer::Create(fbSpec));

		std::shared_ptr<Shader> shader;
		std::shared_ptr<Shader> shaderSimpleTexture;
		std::shared_ptr<Shader> shaderSimpleColor;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Texture> textureLight;

		shader.reset(Shader::Create("res/shaders/phong.glsl"));
		shaderSimpleColor.reset(Shader::Create("res/shaders/simpleColor.glsl"));
		shaderSimpleTexture.reset(Shader::Create("res/shaders/simpleTexture.glsl"));
		texture.reset(Texture::Create("res/textures/uv_mapper.jpg"));
		textureLight.reset(Texture::Create("res/textures/white.png"));

		m_Scene = std::make_shared<Scene>();

		auto quad = m_Scene->CreateEntity("Unlit Quad");
		quad.AddComponent<MeshRenderer>(PrimitiveMesh::Quad(), shaderSimpleTexture, texture);
		
		auto cube = m_Scene->CreateEntity("Lit Quad");
		cube.GetComponent<Transform>().Position = { -2.0f, 0.0f, 0.0f };
		cube.AddComponent<MeshRenderer>(PrimitiveMesh::Cube(), shader, texture).MaterialTeste = std::make_shared<Material>(shader);
		

		camera = m_Scene->CreateEntity("Camera");
		camera.AddComponent<Camera>().ProjectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f);
		auto& cameraTransform = camera.GetComponent<Transform>();
		cameraTransform.Position = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraTransform.Rotation = glm::radians(glm::vec3(0.0f, -90.0f, 0.0f));
		
		auto light = m_Scene->CreateEntity("Light");
		light.AddComponent<Light>().Color = {1.0f, 1.0f, 1.0f};
		auto& lightTransform = light.GetComponent<Transform>();
		lightTransform.Position = { -2.0f, 0.0f, 1.0f };
		lightTransform.Scale = { .1f,.1f,.1f };
		lightTransform.Rotation = { 0.0f,glm::radians(-90.0f), 0.0f };
		light.AddComponent<MeshRenderer>(PrimitiveMesh::Cube(), shaderSimpleColor, texture);

		inspector = std::make_unique<InspectorPanel>();
		m_Panels.push_back(inspector.get());
		m_Panels.push_back(new HierarchyPanel(m_Scene, inspector.get()));
		m_Panels.push_back(new SceneViewportPanel(m_FB, camera));
	}

	void EditorLayer::OnUpdate()
	{
		m_FB->Bind();
		RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1 });
		RenderCommand::Clear();

		EditorCamera();

		m_Scene->OnUpdate();

		m_FB->Unbind();
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

	// rework in the future to normalize input yadda yadda ya

	void EditorLayer::EditorCamera()
	{
		auto& transform = camera.GetComponent<Transform>();
		glm::vec3 pos = transform.Position;
		
		float velocity = 3.0f * Time::GetDeltaTime();

		if (Input::IsKeyPressed(Key::W))
			pos += transform.Front * velocity;
		else if (Input::IsKeyPressed(Key::S))
			pos -= transform.Front * velocity;

		if (Input::IsKeyPressed(Key::D))
			pos += transform.Right * velocity;
		else if (Input::IsKeyPressed(Key::A))
			pos -= transform.Right * velocity;

		if (Input::IsKeyPressed(Key::E))
			pos.y += velocity;
		else if (Input::IsKeyPressed(Key::Q))
			pos.y -= velocity;

		transform.Position = pos;

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
			glm::vec3 rot = transform.Rotation;

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
			transform.Rotation = rot;
		}
	}
}