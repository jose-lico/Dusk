#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"
#include "InspectorPanel.h"

namespace DuskEngine
{
	class HierarchyPanel : public Panel
	{
	public:
		HierarchyPanel(Ref<Scene>& scene, InspectorPanel& inspector);
		~HierarchyPanel();

		virtual void OnImGuiRender() override;
	private:
		Ref<Scene> m_Scene;
		InspectorPanel* m_Inspector;

		std::vector<bool> m_SelectableStatus;
		std::vector<Entity*> m_SelectedEntities;

		friend class Scene;
	};
}