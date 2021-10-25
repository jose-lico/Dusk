#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"

class HierarchyPanel : public PanelBase
{
public:
	HierarchyPanel(std::shared_ptr<DuskEngine::SceneEntt>& scene);
	~HierarchyPanel() = default;

	virtual void OnImGuiRender() override;
private:
	std::shared_ptr<DuskEngine::SceneEntt> m_SceneEntt;
	friend class SceneEntt;
};