#include "HierarchyPanel.h"

#include "InspectorPanel.h"
#include "SceneViewportPanel.h"
#include "DuskEditor/Utils/EditorMeta.h"

#include "Core/ECS/Components/Meta.h"
#include "Utils/Serialization/Yaml.h"

#include "imgui/imgui.h"
#include "IconsForkAwesome.h"

#include <filesystem>
#include <fstream>

namespace DuskEngine
{
	HierarchyPanel::HierarchyPanel(Ref<Scene>& scene, InspectorPanel& inspector, SceneViewportPanel& viewport, const std::string& projectPath)
		:m_Scene(scene), m_ProjectPath(projectPath)
	{
		m_SelectableStatus.resize(m_Scene->m_Registry.size());
		inspector.SelectedEntities(m_SelectedEntities);
		viewport.SelectedEntities(m_SelectedEntities);

		auto& registry = m_Scene->m_Registry;
		auto view = registry.view<Meta>();
		for (entt::entity entity : view)
		{
			Entity(entity, m_Scene.get()).AddComponent<EditorMeta>();
		}

		if (!std::filesystem::is_directory(m_ProjectPath + "/.editor") || !std::filesystem::exists(m_ProjectPath + "/.editor"))
			std::filesystem::create_directory(m_ProjectPath + "/.editor");

		if(std::filesystem::exists(m_ProjectPath + "/.editor/hierarchy.meta"))
		{
			// Load EditorMeta
			YAML::Node hierarchy = YAML::LoadFile(m_ProjectPath + "/.editor/hierarchy.meta");

			auto entities = hierarchy["Entities"];
			for (auto entity : entities)
			{
				uuids::uuid handle = entity["Entity"].as<uuids::uuid>();
				bool visible = entity["Visible"].as<bool>();
				bool clickable = entity["Clickable"].as<bool>();
				bool locked = entity["Locked"].as<bool>();

				for (entt::entity ent : view)
				{
					auto& meta = m_Scene->m_Registry.get<Meta>(ent);
					if(meta.entityHandle == handle)
					{
						auto& editor = m_Scene->m_Registry.get<EditorMeta>(ent);
						editor.Visible = visible;
						editor.Clickable = clickable;
						editor.Locked = locked;
					}
				}
			}
		}
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

		for (entt::entity entity : view)
		{
			auto& meta = m_Scene->m_Registry.get<Meta>(entity);

			bool changedStyle = false;
			if(m_SelectableStatus[entityIndex])
			{
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, { 0.12f, 0.12f, 0.12f, 1.0f });
				changedStyle = true;
			}

			
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
			
			if(changedStyle)
				ImGui::PopStyleColor();

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

			if(ImGui::IsItemHovered())
			{
				auto& editor = m_Scene->m_Registry.get<EditorMeta>(entity);

				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 15.0f); 
				ImGui::Text(editor.Visible ? ICON_FK_EYE : ICON_FK_EYE_SLASH);
				if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(0))
				{
					editor.Visible = !editor.Visible;

					if (!std::filesystem::is_directory(m_ProjectPath + "/.editor") || !std::filesystem::exists(m_ProjectPath + "/.editor"))
						std::filesystem::create_directory(m_ProjectPath + "/.editor");

					if (!std::filesystem::exists(m_ProjectPath + "/.editor/hierarchy.meta"))
						std::ofstream file(m_ProjectPath + "/.editor/hierarchy.meta");

					YAML::Node hierarchy = YAML::LoadFile(m_ProjectPath + "/.editor/hierarchy.meta");
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
					std::ofstream fout(m_ProjectPath + "/.editor/hierarchy.meta");
					fout << out.c_str();
				}
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

	void HierarchyPanel::SetScene(Ref<Scene>& scene)
	{
		m_Scene = scene;

		m_SelectedEntities.resize(0);
		m_SelectableStatus.resize(m_Scene->m_Registry.size());
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
}