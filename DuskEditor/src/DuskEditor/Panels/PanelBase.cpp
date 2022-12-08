#include "PanelBase.h"

#include "imgui/imgui.h"

namespace DuskEngine
{
	void Panel::CheckFocus()
	{
		if(ImGui::IsWindowHovered() && ImGui::IsMouseReleased(1))
		{
			ImGui::SetWindowFocus();
		}
	}
}