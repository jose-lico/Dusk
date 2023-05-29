#include "HierarchyPanel.h"

#include "InspectorPanel.h"
#include "SceneViewportPanel.h"
#include "DuskEditor/Utils/EditorMeta.h"

#include "Core/ECS/Components/Meta.h"
#include "Utils/Serialization/Yaml.h"

#include "imgui/imgui.h"
#include "IconsForkAwesome.h"

#include <fstream>

namespace DuskEngine
{
	HierarchyPanel::HierarchyPanel(Scene& scene, InspectorPanel& inspector, SceneViewportPanel& viewport, const std::filesystem::path& projectPath)
		:m_Scene(&scene), m_ProjectPath(projectPath)
	{
		m_SelectableStatus.resize(m_Scene->m_Registry.size());
		m_Inspector = &inspector;

		// let inspector and viewport know which entities are selected
		m_Inspector->SetSelectedEntities(m_SelectedEntities);
		m_Inspector->SetSelectableStatus(m_SelectableStatus);
		viewport.SelectedEntities(m_SelectedEntities);

		PrepareScene();
	}

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin(ICON_FK_TH_LIST "  Scene Hierarchy");
		CheckFocus();

		ImGui::Text("%s", m_Scene->m_Name);

		ImGui::Separator();

		bool deselected = false;
		bool selectedThisFrame = false;
		bool unselectRequested = false;

		// If mouse is left clikced, request an unselection (can be overwritten if clikced on an entity)
		if (ImGui::IsMouseReleased(0) && ImGui::IsWindowHovered())
			unselectRequested = true;

		auto& registry = m_Scene->m_Registry;
		auto view = registry.view<Meta>();
		
		int32_t entityIndex = 0;
		for (entt::entity entity : view)
		{
			auto& meta = m_Scene->m_Registry.get<Meta>(entity);

			bool changedStyle = false;
			// if an entity is selected, change its hovered color to be the same as when selected
			if(m_SelectableStatus[entityIndex])
			{
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, { 0.12f, 0.12f, 0.12f, 1.0f });
				changedStyle = true;
			}

			ImGui::PushID(entityIndex);
			if (ImGui::Selectable(meta.name.c_str(), m_SelectableStatus[entityIndex]))
			{
				m_Inspector->SetInspectionType(InspectionType::Entity);
				selectedThisFrame = true;

				// if shift was held, select every entity between current and "furthest" one (direction doesnt matter, only "distance" in registry)
				if(ImGui::GetIO().KeyShift)
				{
					SelectBlock(entity);
				}
				// If shift and ctrl were not held, deselect all (DEFAULT behaviour)
				else if (!ImGui::GetIO().KeyCtrl)
				{
					std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
					m_SelectedEntities.resize(0);
				}
				// if ctrl and selecting a selected entity, deselect it
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

				// if current entity is not selected and did not selected in this frame, select it (DEFAULT behaviour)
				if (!m_SelectableStatus[entityIndex] && !deselected)
				{
					m_SelectableStatus[entityIndex] = true;
					m_SelectedEntities.push_back(Entity(entity, m_Scene));
				}
			}
			ImGui::PopID();

			// if the syle was chnaged because this entity was selected, pop it
			if(changedStyle)
				ImGui::PopStyleColor();

			// context menu to delete current entity/entities if multiple selected 
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::GetIO().KeyShift)
				{
					SelectBlock(entity);
				}
				// if current entity not select and ctrl/shift not held, deselect all (DEFAULT behaviour)
				else if (m_SelectableStatus[entityIndex] == false && !ImGui::GetIO().KeyCtrl)
				{
					std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
					m_SelectedEntities.resize(0);
				}

				// if current entity not selected, select it
				if (!m_SelectableStatus[entityIndex])
				{
					m_SelectableStatus[entityIndex] = true;
					m_SelectedEntities.push_back(Entity(entity, m_Scene));
				}

				// delete entity/entities
				if (ImGui::MenuItem(m_SelectedEntities.size() == 1 ? "Delete Entity" : "Delete Entities"))
				{
					for (Entity& selectedEntity : m_SelectedEntities)
					{
						m_Scene->DestroyEntity(selectedEntity.m_EntityHandle);
						TRACE("Deleted");
					}

					std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
					m_SelectableStatus.resize(m_Scene->m_Registry.size());
					m_SelectedEntities.resize(0);
					ImGui::EndPopup();
					break;
				}
				ImGui::EndPopup();
			}

			if(ImGui::IsItemHovered())
			{
				auto& editor = m_Scene->m_Registry.get<EditorMeta>(entity);

				// visible button
				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 15.0f); 
				ImGui::Text(editor.Visible ? ICON_FK_EYE : ICON_FK_EYE_SLASH);

				// alternative way to detected click, using text, since button would be covered by selectable
				if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(0))
				{
					editor.Visible = !editor.Visible;

					// serialize editor metas to file (could probably append somehow)
					YAML::Emitter out;
					out << YAML::BeginMap;
					out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
					
					for (entt::entity ent : view)
					{
						auto& ed = m_Scene->m_Registry.get<EditorMeta>(ent);
						auto& m = m_Scene->m_Registry.get<Meta>(ent);
						out << YAML::BeginMap;
						out << YAML::Key << "Entity" << YAML::Value << m.entityHandle;
						out << YAML::Key << "Visible" << YAML::Value << ed.Visible;
						out << YAML::Key << "Clickable" << YAML::Value << ed.Clickable;
						out << YAML::Key << "Locked" << YAML::Value << ed.Locked;
						out << YAML::EndMap;
					}
					
					out << YAML::EndMap;

					std::filesystem::create_directory(m_ProjectPath / "/.editor");
					std::ofstream fout(m_ProjectPath / "/.editor/hierarchy.meta");
					
					fout << out.c_str();
				}
			}

			entityIndex++;
		}

		// if an unselected was requested with left click on void space and no entity was selected
		if (unselectRequested && !selectedThisFrame)
		{
			std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
			m_SelectedEntities.resize(0);
		}

		// if right click on void
		if (ImGui::BeginPopupContextWindow(0,1,false))
		{
			// reset selected entities
			std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
			m_SelectedEntities.resize(0);

			if (ImGui::MenuItem("Create Empty Entity"))
				Entity emptyEntity = CreateNewEntity("New Entity");

			if (ImGui::BeginMenu("Create Light"))
			{
				if (ImGui::MenuItem("Point Light"))
				{
					Entity lightEntity = CreateNewEntity("New Point Light");
					lightEntity.AddComponent<Light>().type = LightType::Point;

					// TODO: While gizmos dont exist, lights have by default a MeshRenderer component with a default cube
					lightEntity.GetComponent<Transform>().scale = { 0.1f, 0.1f, 0.1f };
					MeshRenderer& meshRenderer = lightEntity.AddComponent<MeshRenderer>();
					meshRenderer.materialHandle = 0;
					meshRenderer.meshHandle = 1;
				}

				if (ImGui::MenuItem("Directional Light"))
				{
					Entity lightEntity = CreateNewEntity("New Directional Light");
					lightEntity.AddComponent<Light>().type = LightType::Directional;

					// TODO: While gizmos dont exist, lights have by default a MeshRenderer component with a default cube
					lightEntity.GetComponent<Transform>().scale = { 0.1f, 0.1f, 0.1f };
					MeshRenderer& meshRenderer = lightEntity.AddComponent<MeshRenderer>();
					meshRenderer.materialHandle = 0;
					meshRenderer.meshHandle = 1;
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void HierarchyPanel::SetScene(Scene& scene)
	{
		m_Scene = &scene;

		m_SelectedEntities.resize(0);
		m_SelectableStatus.resize(m_Scene->m_Registry.size());

		PrepareScene();
	}

	Entity HierarchyPanel::CreateNewEntity(const std::string& name)
	{
		Entity newEntity = m_Scene->CreateEntity(name);
		newEntity.AddComponent<EditorMeta>();

		m_SelectableStatus.resize(m_Scene->m_Registry.size());
		std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);

		m_SelectableStatus[0] = true;
		m_SelectedEntities.push_back(newEntity);

		return newEntity;
	}

	void HierarchyPanel::SelectBlock(const entt::entity& entity)
	{
		auto& registry = m_Scene->m_Registry;
		auto view = registry.view<Meta>();

		if (m_SelectedEntities.size() > 0)
		{
			Entity farthestEntity = m_SelectedEntities[0];
			int32_t farthestEntityDistance = abs((int32_t)entity - (int32_t)farthestEntity.m_EntityHandle);

			for (Entity& selectedEntity : m_SelectedEntities)
			{
				int32_t distance = abs((int32_t)entity - (int32_t)selectedEntity.m_EntityHandle);
				if (distance > farthestEntityDistance)
				{
					farthestEntity = selectedEntity;
					farthestEntityDistance = distance;
				}
			}

			bool isFarthestEntityUp = (int32_t)entity - (int32_t)farthestEntity.m_EntityHandle > 0 ? true : false;

			std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
			m_SelectedEntities.resize(0);

			int32_t index = 0;
			for (auto& ent : view)
			{
				if (isFarthestEntityUp)
				{
					if (ent <= entity && ent >= farthestEntity.m_EntityHandle)
					{
						m_SelectableStatus[index] = true;
						m_SelectedEntities.push_back(Entity(ent, m_Scene));
					}
				}
				else
				{
					if (ent >= entity && ent <= farthestEntity.m_EntityHandle)
					{
						m_SelectableStatus[index] = true;
						m_SelectedEntities.push_back(Entity(ent, m_Scene));
					}
				}

				index++;
			}
		}
	}

	void HierarchyPanel::PrepareScene()
	{
		auto& registry = m_Scene->m_Registry;
		auto view = registry.view<Meta>();

		// Add to each entity a meta component
		for (entt::entity entity : view)
		{
			Entity(entity, m_Scene).AddComponent<EditorMeta>();
		}

		// Get editor meta information
		if (std::filesystem::exists(m_ProjectPath / "/.editor/hierarchy.meta"))
		{
			YAML::Node hierarchy = YAMLLoadFile(m_ProjectPath / "/.editor/hierarchy.meta");

			YAML::Node entities = hierarchy["Entities"];
			for (YAML::Node& entity : entities)
			{
				if (entity["Entity"])
				{
					uuids::uuid handle = YAMLDeserialize<uuids::uuid>(entity, "Entity", uuids::uuid());

					for (entt::entity ent : view)
					{
						Meta& meta = m_Scene->m_Registry.get<Meta>(ent);
						if (meta.entityHandle == handle)
						{
							EditorMeta& editor = m_Scene->m_Registry.get<EditorMeta>(ent);
							editor.Visible = YAMLDeserialize<bool>(entity, "Visible", true);
							editor.Clickable = YAMLDeserialize<bool>(entity, "Clickable", true);
							editor.Locked = YAMLDeserialize<bool>(entity, "Locked", false);
							break;
						}
					}
				}
			}
		}
	}
}
