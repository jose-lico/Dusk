#include "ScenePanel.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

ScenePanel::ScenePanel(std::shared_ptr<DuskEngine::FrameBuffer>& fb, DuskEngine::Camera& camera)
{
	m_Camera = &camera;
	m_FB = fb;
	m_ViewportSize = glm::vec2(0.0f);
}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
	ImGui::Begin("Viewport");
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	if (m_ViewportSize != *(glm::vec2*)&viewportSize)
	{
		m_ViewportSize = { viewportSize.x, viewportSize.y };
		m_FB->Resize(m_ViewportSize);
		m_Camera->SetProjection(glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.01f, 100.0f));
	}
	ImGui::Image((void*)m_FB->GetColorAttachmentID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
	ImGui::PopStyleVar();
}
