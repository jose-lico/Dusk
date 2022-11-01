#include "DebugPanel.h"

#include "Utils/Profiling/Timer.h"

#include "imgui/imgui.h"

#include <string>

namespace DuskEngine
{
	void GoDown(TimerNode* node);

	DebugPanel::DebugPanel()
	{
		//m_RootTimer = Timer::GetRootTimer();
	}
	DebugPanel::~DebugPanel()
	{
	}

	void DebugPanel::OnImGuiRender()
	{
		ImGui::Begin("Debug");
		CheckFocus();
		auto rootTimer = Timer::GetRootTimer();
		auto duration = rootTimer->Duration.count() / 1000000.0f;
		std::string message = rootTimer->Name + " " + std::to_string(duration) + " ms";

		// possibly need to add styling so they arent highlitable/selectable

		if (ImGui::TreeNodeEx(message.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
		{
			GoDown(rootTimer);
			ImGui::TreePop();
		}

		ImGui::End();
	}

	void GoDown(TimerNode* node)
	{
		for each (auto timer in node->Children)
		{
			auto duration = timer->Duration.count() / 1000000.0f;
			std::string message = timer->Name + " " + std::to_string(duration) + " ms";
			if (ImGui::TreeNodeEx(message.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
			{
				GoDown(timer);
				ImGui::TreePop();
			}
		}
	}
}
