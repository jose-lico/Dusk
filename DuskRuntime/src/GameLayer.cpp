#include "GameLayer.h"

namespace DuskEngine
{
	GameLayer::GameLayer()
	{
		m_Scene = MakeRef<Scene>();

		SceneSerializer::DeserializeText(m_Scene, "res/scenes/scene.yaml");

		m_Camera = *m_Scene->GetMainCamera();
	}

	void GameLayer::OnUpdate()
	{
		m_Scene->OnUpdate();

		GameLayer::Camera();
	}

	void GameLayer::Camera()
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
	}
}