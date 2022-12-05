#pragma once
#include "PanelBase.h"

#include "Utils/Memory/Memory.h"

#include <vector>

namespace DuskEngine
{
	class Scene;
	class InspectorPanel;
	class SceneViewportPanel;
	class Entity;

	class HierarchyPanel : public Panel
	{
	public:
		HierarchyPanel(Ref<Scene>& scene, InspectorPanel& inspector, SceneViewportPanel& viewport);
		~HierarchyPanel() = default;

		virtual void OnImGuiRender() override;

		void SetScene(Ref<Scene>& scene);
	private:
		Ref<Scene> m_Scene;

		std::vector<bool> m_SelectableStatus;
		std::vector<Entity> m_SelectedEntities;

		friend class Scene;
		friend class Entity;
	};
}