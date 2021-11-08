#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"
#include "InspectorPanel.h"

namespace DuskEngine
{
	class HierarchyPanel : public Panel
	{
	public:
		HierarchyPanel(std::shared_ptr<Scene>& scene, InspectorPanel* inspector);
		~HierarchyPanel() = default;

		virtual void OnImGuiRender() override;
	private:
		std::shared_ptr<Scene> m_Scene;
		friend class Scene;

		InspectorPanel* m_Inspector;
		Entity* m_InspectedEntity = nullptr;
	};
}