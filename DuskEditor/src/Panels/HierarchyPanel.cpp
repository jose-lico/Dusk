#include "HierarchyPanel.h"

#include <imgui/imgui.h>
#include <IconsForkAwesome.h>

namespace DuskEngine
{
	HierarchyPanel::HierarchyPanel(Ref<Scene>& scene, InspectorPanel& inspector)
		:m_Scene(scene), m_Inspector(&inspector)
	{
		m_SelectableStatus.resize(m_Scene->m_Registry.size());
		m_Inspector->SelectedEntities(m_SelectedEntities);
	}

	HierarchyPanel::~HierarchyPanel()
	{
		// delete entities ptrs
	}

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin(ICON_FK_TH_LIST " Scene Hierarchy");
		CheckFocus();

		int entityIndex = 0;
		m_Scene->m_Registry.each([&](auto entityID)
			{
				auto& meta = m_Scene->m_Registry.get<Meta>(entityID);
				if (ImGui::Selectable(meta.name.c_str(), m_SelectableStatus[entityIndex]))
				{
					if (!ImGui::GetIO().KeyCtrl)
					{
						std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
						m_SelectedEntities.resize(0);
					}
					if(!m_SelectableStatus[entityIndex])
					{
						m_SelectableStatus[entityIndex] = true;
						m_SelectedEntities.push_back(new Entity(entityID, m_Scene.get()));
					}
				}
				entityIndex++;
			});

		ImGui::End();
	}
}