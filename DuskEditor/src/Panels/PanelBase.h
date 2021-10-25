#pragma once

#include <imgui.h>

namespace DuskEngine
{
	class PanelBase
	{
	public:
		PanelBase() = default;
		~PanelBase() = default;

		virtual void OnImGuiRender() = 0;
	};
}
