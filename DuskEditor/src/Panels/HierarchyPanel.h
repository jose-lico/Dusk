#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"
#include "InspectorPanel.h"

namespace DuskEngine
{
	class HierarchyPanel : public PanelBase
	{
	public:
		HierarchyPanel(std::shared_ptr<SceneEntt>& scene, InspectorPanel* inspector);
		~HierarchyPanel() = default;

		virtual void OnImGuiRender() override;
	private:
		std::shared_ptr<SceneEntt> m_SceneEntt;
		friend class SceneEntt;

		InspectorPanel* m_Inspector;
		Entity* m_InspectedEntity = nullptr;
	};
}