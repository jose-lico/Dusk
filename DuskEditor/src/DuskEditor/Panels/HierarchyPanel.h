#pragma once
#include "PanelBase.h"

#include "entt/entity/entity.hpp"

#include <vector>
#include <string>
#include <filesystem>

namespace DuskEngine
{
	class Scene;
	class InspectorPanel;
	class SceneViewportPanel;
	class Entity;

	class HierarchyPanel : public Panel
	{
	public:
		HierarchyPanel(Scene& scene, InspectorPanel& inspector, SceneViewportPanel& viewport, const std::filesystem::path& projectPath);
		~HierarchyPanel() = default;

		virtual void OnImGuiRender() override;

		void SetScene(Scene& scene);
	private:
		Entity CreateNewEntity(const std::string& name);
		void SelectBlock(const entt::entity& entity);
		void PrepareScene();

		Scene* m_Scene;
		InspectorPanel* m_Inspector;

		std::vector<bool> m_SelectableStatus;
		std::vector<Entity> m_SelectedEntities;
		std::filesystem::path m_ProjectPath;

		friend class Scene;
		friend class Entity;
	};
}