#include "HierarchyPanel.h"

#include "imgui.h"

namespace DuskEngine
{
	HierarchyPanel::HierarchyPanel(std::shared_ptr<Scene>& scene, InspectorPanel* inspector)
		:m_Scene(scene), m_Inspector(inspector)
	{
	}

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");
		m_Scene->m_Registry.each([&](auto entityID)
			{
				auto& meta = m_Scene->m_Registry.get<Meta>(entityID);
				if (ImGui::Button(meta.Name.c_str()))
				{
					m_InspectedEntity = new Entity(entityID, m_Scene.get());
					m_Inspector->SetEntity(m_InspectedEntity);
				}
			});
		ImGui::End();
	}
}