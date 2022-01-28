#pragma once
#include "PanelBase.h"

namespace DuskEngine
{
	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		virtual void OnImGuiRender() override;
	};
}