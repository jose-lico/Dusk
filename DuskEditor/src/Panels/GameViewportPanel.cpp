#include "GameViewportPanel.h"

#include "Core/ECS/Components/Camera.h"
#include "Core/Renderer/Resources/Framebuffer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "IconsForkAwesome.h"

namespace DuskEngine
{
	GameViewportPanel::GameViewportPanel(Ref<FrameBuffer>& fb, Entity camera, bool* playing)
		:m_Playing(playing), m_FB(fb), m_Camera(camera)
	{
		m_ViewportSize = glm::vec2(0.0f);
	}

	void GameViewportPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin(ICON_FK_GAMEPAD "  Game", nullptr);
		CheckFocus();
		if(*m_Playing)
		{
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *(glm::vec2*)&viewportSize)
			{
				m_ViewportSize = { viewportSize.x, viewportSize.y };
				m_FB->Resize(m_ViewportSize);
				m_Camera.GetComponent<Camera>().projectionMatrix = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.01f, 100.0f);
			}
			ImGui::Image((void*)m_FB->GetColorAttachmentID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });	
		}
		else
		{
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *(glm::vec2*)&viewportSize)
			{
				m_ViewportSize = { viewportSize.x, viewportSize.y };
				m_FB->Resize(m_ViewportSize);
				m_Camera.GetComponent<Camera>().projectionMatrix = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.01f, 100.0f);
			}
			ImGui::Image((void*)m_FB->GetColorAttachmentID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}

		ImGui::PopStyleVar();

		ImGui::End();
	}
}