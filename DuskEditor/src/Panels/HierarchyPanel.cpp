#include "HierarchyPanel.h"

#include <imgui/imgui.h>
#include <IconsForkAwesome.h>

namespace DuskEngine
{
	HierarchyPanel::HierarchyPanel(Ref<Scene>& scene, InspectorPanel& inspector, SceneViewportPanel& viewport)
		:m_Scene(scene)
	{
		m_SelectableStatus.resize(m_Scene->m_Registry.size());
		inspector.SelectedEntities(m_SelectedEntities);
		viewport.SelectedEntities(m_SelectedEntities);
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

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("New Light"))
			{
				auto light = m_Scene->CreateEntity("New Light");
				std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
				m_SelectableStatus.resize(m_Scene->m_Registry.size());
				light.AddComponent<Light>();

				auto& lightTransform = light.GetComponent<Transform>();
				lightTransform.position = { -2.0f, 1.0f, 1.0f };
				lightTransform.scale = { 0.1f, 0.1f, 0.1f };
				light.AddComponent<MeshRenderer>(PrimitiveMesh::Cube(), MakeRef<Material>(Shader::Create("res/shaders/simpleColor.glsl")));

				// select currently created entity
			}

			if (ImGui::MenuItem("New Model"))
			{
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}
}