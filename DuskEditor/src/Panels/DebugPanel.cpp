#include "DebugPanel.h"

#include "Utils/Profiling/Timer.h"

#include "imgui/imgui.h"
#include "spdlog/fmt/fmt.h"

#include <string>

namespace DuskEngine
{
	TimerNode* DebugPanel::m_RootTimer = nullptr;

	void GoDownTreeEx(TimerNode* node);
	void GoDownDelete(TimerNode* node);

	DebugPanel::DebugPanel()
	{
		m_RootTimer = Timer::GetRootTimer();
	}

	DebugPanel::~DebugPanel()
	{
		GoDownDelete(m_RootTimer);
		delete m_RootTimer;
	}

	void DebugPanel::OnImGuiRender()
	{
		ImGui::Begin("Debug");
		CheckFocus();
		auto duration = m_RootTimer->Duration.count() / 1000000.0f;
		std::string message = m_RootTimer->Name + ": " + fmt::format("{:.3f}", duration) + " ms";

		// possibly need to add styling so they arent highlitable/selectable

		if (ImGui::TreeNodeEx(message.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			GoDownTreeEx(m_RootTimer);
			ImGui::TreePop();
		}

		ImGui::End();
	}

	void GoDownTreeEx(TimerNode* node)
	{
		for each (auto timer in node->Children)
		{
			auto duration = timer->Duration.count() / 1000000.0f;
			std::string message = timer->Name + ": " + fmt::format("{:.3f}", duration) + " ms";
			if (ImGui::TreeNodeEx(message.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
			{
				GoDownTreeEx(timer);
				ImGui::TreePop();
			}
		}
	}

	void GoDownDelete(TimerNode* node)
	{
		for each (auto timer in node->Children)
		{
			GoDownDelete(timer);
			delete timer;
		}
	}
}
