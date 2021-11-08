#pragma once
#include "PanelBase.h"

namespace DuskEngine
{
	class ConsolePanel : public Panel
	{
	public:
		ConsolePanel() = default;
		~ConsolePanel() = default;
		virtual void OnImGuiRender() override;
	};
}