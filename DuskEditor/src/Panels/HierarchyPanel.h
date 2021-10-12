#pragma once
#include "PanelBase.h"

class HierarchyPanel : public PanelBase
{
public:
	HierarchyPanel() = default;
	~HierarchyPanel() = default;

	virtual void OnImGuiRender() override;
};