#include "RuntimeLayer.h"

#include "Core/Application/Input.h"
#include "Core/Application/Time.h"
#include "Core/Serialization/SceneSerializer.h"
#include "Core/ECS/Components/Camera.h"

#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

namespace DuskEngine
{
	RuntimeLayer::RuntimeLayer()
	{
		m_Scene = MakeRef<Scene>("Runtime Scene");

		SceneSerializer::DeserializeText(m_Scene, "res/scenes/scene.yaml");

		m_Scene->OnAwakeRuntime();

		//m_Camera = *m_Scene->GetMainCamera();
	}

	RuntimeLayer::~RuntimeLayer()
	{
		m_Scene->OnShutdownRuntime();
	}

	void RuntimeLayer::OnUpdate()
	{
		m_Scene->OnUpdateRuntime(true);

		//RuntimeLayer::Camera();
	}

	void RuntimeLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(RuntimeLayer::CameraProj));
	}

	bool RuntimeLayer::CameraProj(WindowResizeEvent& event)
	{
		auto camera = m_Scene->GetMainCamera();
		camera->GetComponent<Camera>().projectionMatrix =
			glm::perspective(glm::radians(45.0f), (float)(event.GetWidth()/event.GetHeight()), 0.01f, 100.0f);

		return true;
	}

	/*void RuntimeLayer::Camera()
	{
		if (!m_IsLeftMousePressed && Input::IsMouseButtonPressed(Mouse::MOUSE_BUTTON_2))
		{
			m_IsLeftMousePressed = true;
			m_HasFirstMousePosition = false;

			Input::SetCursorActive(Cursor::CURSOR_DISABLED);
		}
		else if (!Input::IsMouseButtonPressed(Mouse::MOUSE_BUTTON_2))
		{
			Input::SetCursorActive(Cursor::CURSOR_NORMAL);
			m_IsLeftMousePressed = false;
		}

		if (m_IsLeftMousePressed)
		{
			auto& transform = m_Camera.GetComponent<Transform>();

			float moveSpeed = 3.0f;
			float rotSpeed = 0.5f;

			glm::vec3 moveInput(0.0f);
			glm::vec3 rotInput(0.0f);

			if (Input::IsKeyPressed(Key::W))
				moveInput += transform.front;
			else if (Input::IsKeyPressed(Key::S))
				moveInput -= transform.front;

			if (Input::IsKeyPressed(Key::D))
				moveInput += transform.right;
			else if (Input::IsKeyPressed(Key::A))
				moveInput -= transform.right;

			if (Input::IsKeyPressed(Key::E))
				moveInput += transform.up;
			else if (Input::IsKeyPressed(Key::Q))
				moveInput -= transform.up;

			if (glm::length(moveInput) > 0)
				moveInput = glm::normalize(moveInput);

			transform.position += moveInput * moveSpeed * Time::GetDeltaTime();

			if (!m_HasFirstMousePosition)
			{
				m_LastMousePosition.x = Input::GetMouseX();
				m_LastMousePosition.y = Input::GetMouseY();
				m_HasFirstMousePosition = true;
			}

			rotInput.y = Input::GetMouseX() - m_LastMousePosition.x;
			rotInput.x = m_LastMousePosition.y - Input::GetMouseY();

			m_LastMousePosition.x = Input::GetMouseX();
			m_LastMousePosition.y = Input::GetMouseY();

			rotInput.x *= rotSpeed * Time::GetDeltaTime();
			rotInput.y *= rotSpeed * Time::GetDeltaTime();

			transform.rotation += rotInput;
		}
	}*/
	
	void DropCallback(GLFWwindow* window, int count, const char** paths)
	{
		
	}
}