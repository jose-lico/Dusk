#include "HierarchyPanel.h"

#include "imgui.h"

HierarchyPanel::HierarchyPanel(std::shared_ptr<DuskEngine::SceneEntt>& scene)
	:m_SceneEntt(scene)
{
}

void HierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Hierarchy");
	m_SceneEntt->m_Registry.each([&](auto entityID)
		{
			if(ImGui::Button("Entity"))
			{
			}
		});
	ImGui::End();
}