#include "pch.h"
#include "Toolbar.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace DuskEngine
{
	Toolbar::Toolbar(bool* playing)
		:m_Playing(playing)
	{
	}

	void Toolbar::OnImGuiRender()
	{
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeFlagsMask_;
		ImGui::SetNextWindowClass(&window_class);
		ImGui::Begin("##Toolbar");
		CheckFocus();


		ImGui::End();
	}
}