#include "HierarchyPanel.h"

#include "InspectorPanel.h"
#include "SceneViewportPanel.h"

#include "Core/ECS/Components/Meta.h"

#include "imgui/imgui.h"
#include "IconsForkAwesome.h"

namespace DuskEngine
{
	HierarchyPanel::HierarchyPanel(Ref<Scene>& scene, InspectorPanel& inspector, SceneViewportPanel& viewport)
		:m_Scene(scene)
	{
		m_SelectableStatus.resize(m_Scene->m_Registry.size());
		inspector.SelectedEntities(m_SelectedEntities);
		viewport.SelectedEntities(m_SelectedEntities);
	}

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin(ICON_FK_TH_LIST "  Scene Hierarchy");
		CheckFocus();

		bool deselected = false;
		bool selectedThisFrame = false;
		bool unselectRequested = false;

		if (ImGui::IsMouseReleased(0) && ImGui::IsWindowHovered())
			unselectRequested = true;

		auto& registry = m_Scene->m_Registry;
		auto view = registry.view<Meta>();
		
		int entityIndex = 0;
		
		for (auto& entity : view)
		{
			auto& meta = m_Scene->m_Registry.get<Meta>(entity);

			ImGui::PushID(entityIndex);
			if (ImGui::Selectable(meta.name.c_str(), m_SelectableStatus[entityIndex]))
			{
				selectedThisFrame = true;

				if(ImGui::GetIO().KeyShift)
				{
					if(m_SelectedEntities.size() > 0 && m_SelectedEntities[0].m_EntityHandle != entity)
					{
						Entity farthestEntity = m_SelectedEntities[0];
						int32_t farthestEntityDistance = abs((int32_t)entity - (int32_t)farthestEntity.m_EntityHandle);
						bool isFarthestEntityUp = (int32_t)entity - (int32_t)farthestEntity.m_EntityHandle > 0 ? true : false; // not used for now

						for (Entity& selectedEntity : m_SelectedEntities)
						{
							int32_t distance = abs((int32_t)entity - (int32_t)selectedEntity.m_EntityHandle);
							if(distance > farthestEntityDistance)
							{
								farthestEntity = selectedEntity;
								farthestEntityDistance = distance;
							}
						}

						std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
						m_SelectedEntities.resize(0);

						int32_t index = 0;
						for (auto& ent : view)
						{
							if(isFarthestEntityUp)
							{
								if (ent <= entity && ent >= farthestEntity.m_EntityHandle)
								{
									m_SelectableStatus[index] = true;
									m_SelectedEntities.push_back(Entity(ent, m_Scene.get()));
								}
							}
							else
							{
								if (ent >= entity && ent <= farthestEntity.m_EntityHandle)
								{
									m_SelectableStatus[index] = true;
									m_SelectedEntities.push_back(Entity(ent, m_Scene.get()));
								}
							}
							
							index++;
						}
					}
				}
				else if (!ImGui::GetIO().KeyCtrl)
				{
					std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
					m_SelectedEntities.resize(0);
				}
				else if(ImGui::GetIO().KeyCtrl && m_SelectableStatus[entityIndex])
				{
					m_SelectableStatus[entityIndex] = false;
					deselected = true;

					int32_t index = 0;
					for (Entity& selectedEntity : m_SelectedEntities)
					{
						if(selectedEntity.m_EntityHandle == entity)
						{
							m_SelectedEntities.erase(m_SelectedEntities.begin() + index);

							break;
						}
						index++;
					}
				}

				if (!m_SelectableStatus[entityIndex] && !deselected)
				{
					m_SelectableStatus[entityIndex] = true;
					m_SelectedEntities.push_back(Entity(entity, m_Scene.get()));
				}
			}
			ImGui::PopID();

			if (ImGui::BeginPopupContextItem())
			{
				if (m_SelectableStatus[entityIndex] == false && !ImGui::GetIO().KeyCtrl)
				{
					std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
					m_SelectedEntities.resize(0);
				}

				if (!m_SelectableStatus[entityIndex])
				{
					m_SelectableStatus[entityIndex] = true;
					m_SelectedEntities.push_back(Entity(entity, m_Scene.get()));
				}

				if (ImGui::MenuItem(m_SelectedEntities.size() == 1 ? "Delete Entity" : "Delete Entities"))
				{
					for (Entity& selectedEntity : m_SelectedEntities)
					{
						m_Scene->DestroyEntity(selectedEntity.m_EntityHandle);
					}

					std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
					m_SelectableStatus.resize(m_Scene->m_Registry.size());
					m_SelectedEntities.resize(0);
				}
				ImGui::EndPopup();
			}

			entityIndex++;
		}

		if (unselectRequested && !selectedThisFrame)
		{
			std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
			m_SelectedEntities.resize(0);
		}

		if (ImGui::BeginPopupContextWindow(0,1,false))
		{
			std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
			m_SelectedEntities.resize(0);

			if (ImGui::MenuItem("Create Empty Entity"))
			{
				Entity emptyEntity = m_Scene->CreateEntity("New Entity");
				SelectNewEntity(emptyEntity);
			}

			if (ImGui::BeginMenu("Create Light"))
			{
				if (ImGui::MenuItem("Point Light"))
				{
					Entity lightEntity = m_Scene->CreateEntity("New Point Light");
					lightEntity.AddComponent<Light>().type = LightType::Point;

					// TODO: While gizmos dont exist, lights have by default a MeshRenderer component with a default cube
					lightEntity.GetComponent<Transform>().scale = { 0.1f, 0.1f, 0.1f };
					MeshRenderer& meshRenderer = lightEntity.AddComponent<MeshRenderer>();
					meshRenderer.materialHandle = 0;
					meshRenderer.meshHandle = 1;

					SelectNewEntity(lightEntity);
				}

				if (ImGui::MenuItem("Directional Light"))
				{
					Entity lightEntity = m_Scene->CreateEntity("New Directional Light");
					lightEntity.AddComponent<Light>().type = LightType::Directional;

					// TODO: While gizmos dont exist, lights have by default a MeshRenderer component with a default cube
					lightEntity.GetComponent<Transform>().scale = { 0.1f, 0.1f, 0.1f };
					MeshRenderer& meshRenderer = lightEntity.AddComponent<MeshRenderer>();
					meshRenderer.materialHandle = 0;
					meshRenderer.meshHandle = 1;

					SelectNewEntity(lightEntity);
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void HierarchyPanel::SetScene(Ref<Scene>& scene)
	{
		m_Scene = scene;

		m_SelectedEntities.resize(0);
		m_SelectableStatus.resize(m_Scene->m_Registry.size());
	}

	void HierarchyPanel::SelectNewEntity(Entity& newEntity)
	{
		std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
		m_SelectableStatus.resize(m_Scene->m_Registry.size());

		m_SelectableStatus[0] = true;
		m_SelectedEntities.push_back(newEntity);
	}
}