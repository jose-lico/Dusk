#pragma once

class PanelBase
{
public:
	PanelBase() = default;
	~PanelBase() = default;

	virtual void OnImGuiRender() = 0;
};