#include "HierarchyPanel.h"

#include "InspectorPanel.h"
#include "SceneViewportPanel.h"

#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/Components/Meta.h"
#include "Platform/OpenGL/Shader.h"
#include "Core/Assets/AssetDatabase.h"

#include "Utils/Rendering/PrimitiveMesh.h"

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

		auto& registry = m_Scene->m_Registry;

		auto view = registry.view<Meta>();

		int entityIndex = 0;
		for (auto& entity : view)
		{
			auto& meta = m_Scene->m_Registry.get<Meta>(entity);
			if (ImGui::Selectable(meta.name.c_str(), m_SelectableStatus[entityIndex]))
			{
				if (!ImGui::GetIO().KeyCtrl)
				{
					std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
					m_SelectedEntities.resize(0);
				}
				if (!m_SelectableStatus[entityIndex])
				{
					m_SelectableStatus[entityIndex] = true;
					m_SelectedEntities.push_back(Entity(entity, m_Scene.get()));
				}
			}

			if (ImGui::BeginPopupContextItem())
			{
				if(m_SelectableStatus[entityIndex] == false && !ImGui::GetIO().KeyCtrl)
				{
					std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
					m_SelectedEntities.resize(0);
				}

				m_SelectableStatus[entityIndex] = true;
				bool addToSelectedEntities = true;
				for (Entity& selectedEntity : m_SelectedEntities)
				{
					if(selectedEntity.m_EntityHandle == entity)
					{
						addToSelectedEntities = false;
						break;
					}
				}

				if(addToSelectedEntities)
					m_SelectedEntities.push_back(Entity(entity, m_Scene.get()));

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

		if (ImGui::BeginPopupContextWindow(0,1,false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Scene->CreateEntity("New Entity");
				std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
				m_SelectableStatus.resize(m_Scene->m_Registry.size());

				// select currently created entity
			}

			//if (ImGui::MenuItem("Create Light"))
			//{
			//	auto light = m_Scene->CreateEntity("New Light");
			//	std::fill(m_SelectableStatus.begin(), m_SelectableStatus.end(), 0);
			//	m_SelectableStatus.resize(m_Scene->m_Registry.size());
			//	light.AddComponent<Light>();

			//	auto& lightTransform = light.GetComponent<Transform>();
			//	lightTransform.position = { -2.0f, 1.0f, 1.0f };
			//	lightTransform.scale = { 0.1f, 0.1f, 0.1f };
			//	auto mesh = PrimitiveMesh::Cube();
			//	auto material = AssetManager::LoadMaterial(AssetManager::GetUUID("res/materials/lightMaterial.material"));
			//	//light.AddComponent<MeshRenderer>(mesh, material);

			//	// select currently created entity
			//}

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
}