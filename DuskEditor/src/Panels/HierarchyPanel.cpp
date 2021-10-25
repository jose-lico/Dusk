#include "HierarchyPanel.h"

#include "imgui.h"

HierarchyPanel::HierarchyPanel(std::shared_ptr<DuskEngine::SceneEntt>& scene, InspectorPanel* inspector)
	:m_SceneEntt(scene), m_Inspector(inspector)
{
}

void HierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Hierarchy");
	m_SceneEntt->m_Registry.each([&](auto entityID)
		{
			std::string s = "Entity " + std::to_string((unsigned int)entityID);
			if(ImGui::Button(s.c_str()))
			{
				m_InspectedEntity = new DuskEngine::Entity(entityID, m_SceneEntt.get());
				m_Inspector->SetEntity(m_InspectedEntity);
			}
		});
	ImGui::End();
}