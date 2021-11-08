#include "ConsolePanel.h"

#include <imgui.h>

namespace DuskEngine
{
	void ConsolePanel::OnImGuiRender()
	{
		ImGui::Begin("Console");
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