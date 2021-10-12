#pragma once

#include <imgui.h>

class PanelBase
{
public:
	PanelBase() = default;
	~PanelBase() = default;

	virtual void OnImGuiRender() = 0;
};