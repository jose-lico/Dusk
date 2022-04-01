#include "PanelBase.h"

#include "imgui/imgui.h"

namespace DuskEngine
{
	void Panel::CheckFocus()
	{
		if(ImGui::IsWindowHovered() && ImGui::GetIO().MouseClicked[1])
		{
			ImGui::SetWindowFocus();
		}
	}
}