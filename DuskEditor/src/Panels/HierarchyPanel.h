#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"
#include "InspectorPanel.h"
#include "SceneViewportPanel.h"

namespace DuskEngine
{
	class HierarchyPanel : public Panel
	{
	public:
		HierarchyPanel(Ref<Scene>& scene, InspectorPanel& inspector, SceneViewportPanel& viewport);
		~HierarchyPanel();

		virtual void OnImGuiRender() override;
	private:
		Ref<Scene> m_Scene;

		std::vector<bool> m_SelectableStatus;
		std::vector<Entity*> m_SelectedEntities;

		friend class Scene;
	};
}