#include "SceneViewportPanel.h"

#include "Core/ECS/Components/Camera.h"
#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/EditorCamera.h"
#include "Core/Renderer/Resources/Framebuffer.h"
#include "Core/Application/Time.h"
#include "Core/Application/Input.h"

#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "glm/gtc/type_ptr.hpp"
#include "IconsForkAwesome.h"

namespace DuskEngine
{
	SceneViewportPanel::SceneViewportPanel(Ref<FrameBuffer>& fb, EditorCamera* camera)
	{ 
		m_ViewportSize = glm::vec2(0.0f);
		m_FB = fb;
		m_Camera = camera;
	}

	static bool stats = false;
	void SceneViewportPanel::OnImGuiRender()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin(ICON_FK_EYE "  Viewport", nullptr, window_flags);
		CheckFocus();
 
		if (ImGui::BeginMenuBar())
		{
            ImGui::MenuItem("Stats", "", &stats);

			ImGui::EndMenuBar();
		}

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportSize)
		{
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_FB->Resize(m_ViewportSize);
			m_Camera->camera.projectionMatrix = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.01f, 100.0f);
		}
		ImGui::Image((void*)m_FB->GetColorAttachmentID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::PopStyleVar();
        ImVec2 work_pos = ImGui::GetWindowPos();
        auto viewportID = ImGui::GetWindowViewport()->ID;

        if (stats && !ImGui::IsWindowCollapsed())
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings 
                | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove;
    
            ImVec2 window_pos = { work_pos.x + 10 , work_pos.y + 50 };
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2());

			ImGui::SetNextWindowViewport(viewportID);
            ImGui::SetNextWindowBgAlpha(0.35f);
            if (ImGui::Begin("Example: Simple overlay", &stats, window_flags))
            {
				ImGui::Text("Frame time: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
				ImGui::Text("FPS: %.1f FPS", ImGui::GetIO().Framerate);
                ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
                ImGui::Separator();
            }
            ImGui::End();
        }
		
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
				ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transformMatrix));

			if(ImGuizmo::IsUsing())
			{
				transform.position = (glm::vec3)transformMatrix[3];
			}
		}

		ImGui::End();
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

			transform.rotation += rotInput;
		}
	}
}