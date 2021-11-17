#include "SceneViewportPanel.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace DuskEngine
{
	SceneViewportPanel::SceneViewportPanel(Ref<FrameBuffer>& fb, Entity camera)
	{
		m_ViewportSize = glm::vec2(0.0f);
		m_FB = fb;
		m_Camera = camera;
	}

	void SceneViewportPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportSize)
		{
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_FB->Resize(m_ViewportSize);
			m_Camera.GetComponent<Camera>().projectionMatrix = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.01f, 100.0f);
		}
		ImGui::Image((void*)m_FB->GetColorAttachmentID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}
}