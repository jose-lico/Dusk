#include "SceneViewportPanel.h"

#include "Core/ECS/Components/Camera.h"
#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/EditorCamera.h"
#include "Core/Application/Time.h"
#include "Core/Application/Input.h"
#include "Core/Events/KeyEvent.h"
#include "Platform/OpenGL/Framebuffer.h"
#include "Platform/OpenGL/OpenGLAPI.h"

#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "glm/gtc/type_ptr.hpp"
#include "IconsForkAwesome.h"

namespace DuskEngine
{
	SceneViewportPanel::SceneViewportPanel(Framebuffer* fb, EditorCamera* camera)
	{ 
		m_ViewportSize = glm::vec2(0.0f);
		m_FB = fb;
		m_Camera = camera;

		m_ImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	}

	static bool stats = false;
	void SceneViewportPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin(ICON_FK_EYE "  Viewport", nullptr);
		CheckFocus();
	
		/*ImGui::Indent();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (ImGui::Button("Hey"))
			stats = !stats;
		ImGui::SameLine();
		ImGui::Button("Hey2");
		ImGui::PopStyleColor();
		ImGui::Unindent();*/
		
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportSize)
		{
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_FB->Width = viewportSize.x;
			m_FB->Height = viewportSize.y;
			OpenGLAPI::ResizeFramebuffer(*m_FB);
			m_Camera->camera.projectionMatrix = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.01f, 100.0f);
		}
		ImGui::Image((ImTextureID)(size_t)m_FB->ColorAttachment, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::PopStyleVar();
		ImVec2 work_pos = ImGui::GetWindowPos();
		auto viewportID = ImGui::GetWindowViewport()->ID;
		
		/*if (ImGui::BeginMenuBar())
		{
			ImGui::MenuItem("Stats", "", &stats);

			ImGui::EndMenuBar();
		}*/

   //     if (stats && !ImGui::IsWindowCollapsed())
   //     {
   //         ImGuiIO& io = ImGui::GetIO();
   //         ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings 
   //             | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove;
   // 
   //         ImVec2 window_pos = { work_pos.x + 10 , work_pos.y + 50 };
   //         ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2());

			//ImGui::SetNextWindowViewport(viewportID);
   //         ImGui::SetNextWindowBgAlpha(0.35f);
   //         if (ImGui::Begin("Example: Simple overlay", &stats, window_flags))
   //         {
			//	// imgui bad stats change later
			//	ImGui::Text("Frame time: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
			//	ImGui::Text("FPS: %.1f FPS", ImGui::GetIO().Framerate);
   //             ImGui::Separator();
   //         }
   //         ImGui::End();
   //     }
		
		if(ImGui::IsWindowFocused())
			MoveEditorCamera();

		if(m_SelectedEntities->size() > 0)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
			
			auto& camera = m_Camera->camera;

			auto& transform = (*m_SelectedEntities)[0].GetComponent<Transform>();
			glm::mat4 transformMatrix = transform.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(camera.viewMatrix), glm::value_ptr(camera.projectionMatrix),
				(ImGuizmo::OPERATION)m_ImGuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(transformMatrix));

			if(ImGuizmo::IsUsing())
			{
				switch (m_ImGuizmoOperation)
				{
				case ImGuizmo::TRANSLATE:
					transform.position = (glm::vec3)transformMatrix[3];
					break;
				case ImGuizmo::ROTATE:
					break;
				case ImGuizmo::SCALE:
					break;
				case ImGuizmo::SCALEU:
					break;
				case ImGuizmo::UNIVERSAL:
					break;
				default:
					break;
				}
			}
		}
		ImGui::End();
	}

	void SceneViewportPanel::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(SceneViewportPanel::Test));
	}

	bool SceneViewportPanel::Test(KeyPressedEvent& e)
	{
		#ifdef DUSK_WINDOWS
		if (!m_IsLeftMousePressed) {
			switch (e.GetKeyCode())
			{
			case Key::Q:
				m_ImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
				return true;
				break;
			case Key::W:
				m_ImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
				return true;
				break;
			case Key::E:
				m_ImGuizmoOperation = ImGuizmo::OPERATION::ROTATE;
				return true;
				break;
			case Key::R:
				m_ImGuizmoOperation = ImGuizmo::OPERATION::SCALE;
				return true;
				break;
			case Key::T:
				m_ImGuizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;
				return true;
				break;
			}
		}
		#endif

		return false;
	}

	void SceneViewportPanel::MoveEditorCamera()
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
			auto& transform = m_Camera->transform;

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

			if(glm::length(moveInput) > 0)
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

			transform.rotation -= rotInput;
		}
	}
}