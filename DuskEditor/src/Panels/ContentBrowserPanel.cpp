#include "pch.h"
#include "ContentBrowserPanel.h"

#include <IconsForkAwesome.h>

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

		ImGui::Text(ICON_FK_BULLHORN "  idk");

		ImGui::End();
	}
}