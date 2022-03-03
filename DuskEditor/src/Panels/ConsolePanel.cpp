#include "ConsolePanel.h"

#include <imgui/imgui.h>
#include <IconsForkAwesome.h>

namespace DuskEngine
{
	void ConsolePanel::OnImGuiRender()
	{
		ImGui::Begin(ICON_FK_TERMINAL "  Console");
        CheckFocus();
		ImGui::BeginTabBar("MyTabBar");
        if (ImGui::BeginTabItem("Engine"))
        {
            ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Editor"))
        {
            ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Application"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
		ImGui::EndTabBar();

		ImGui::End();
	}
}