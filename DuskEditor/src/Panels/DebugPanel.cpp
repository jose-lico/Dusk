#include "DebugPanel.h"

#include "Core/Application/Core.h"
#include "Core/Application/Time.h"
#include "Utils/Profiling/Timer.h"

#include "imgui/imgui.h"
#include "spdlog/fmt/fmt.h"

#include <string>

namespace DuskEngine
{
	void GoDownTreeEx(TimerNode* node);
	void GoDownDelete(TimerNode* node);

	DebugPanel::DebugPanel()
	{
		m_StartupRoot = Timer::GetRootTimer();
	}

	DebugPanel::~DebugPanel()
	{
		GoDownDelete(m_StartupRoot);
		delete m_StartupRoot;
	}

	void DebugPanel::OnImGuiRender()
	{
		ImGui::Begin("Debug");
		CheckFocus();
		auto duration = m_StartupRoot->Duration.count() / 1000000.0f;
		std::string message = m_StartupRoot->Name + ": " + fmt::format("{:.3f}", duration) + " ms";

		// possibly need to add styling so they arent highlitable/selectable

		if (ImGui::TreeNodeEx(message.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			GoDownTreeEx(m_StartupRoot);
			ImGui::TreePop();
		}

		ImGui::Separator();

		if (m_RefreshFPSTimer > 0.25f)
		{
			m_RefreshFPSTimer = 0.0f;
			m_Frametime = Timer::GetRootTimer()->Duration.count() / 1000000.0f;
		}
		else
			m_RefreshFPSTimer += Time::GetDeltaTime();
		
		//if(Timer::GetRootTimer() != nullptr)
			ImGui::Text("%s: %sms (%d FPS)", Timer::GetRootTimer()->Name.c_str(), fmt::format("{:.3f}", m_Frametime).c_str(), (int)(1000.0f / m_Frametime));

		ImGui::End();
	}

	void GoDownTreeEx(TimerNode* node)
	{
		for(auto timer : node->Children)
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
		for(auto timer : node->Children)
		{
			GoDownDelete(timer);
			delete timer;
		}
	}
}
