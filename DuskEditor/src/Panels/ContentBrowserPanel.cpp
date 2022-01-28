#include "pch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace DuskEngine
{
	ContentBrowserPanel::ContentBrowserPanel()
	{
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");
		ImGui::End();
	}
}