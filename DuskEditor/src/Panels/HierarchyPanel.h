#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"
#include "InspectorPanel.h"

class HierarchyPanel : public PanelBase
{
public:
	HierarchyPanel(std::shared_ptr<DuskEngine::SceneEntt>& scene, InspectorPanel* inspector);
	~HierarchyPanel() = default;

	virtual void OnImGuiRender() override;
private:
	std::shared_ptr<DuskEngine::SceneEntt> m_SceneEntt;
	friend class SceneEntt;

	InspectorPanel* m_Inspector;
	DuskEngine::Entity* m_InspectedEntity = nullptr;
};