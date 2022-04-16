#include "pch.h"
#include "GameViewportPanel.h"

#include "imgui/imgui.h"

namespace DuskEngine
{
	GameViewPortPanel::GameViewPortPanel(Ref<FrameBuffer>& fb, Entity camera, bool* playing)
		:m_Playing(playing), m_FB(fb), m_Camera(camera)
	{
		m_ViewportSize = glm::vec2(0.0f);
	}

	void GameViewPortPanel::OnImGuiRender()
	{
		ImGui::Begin("Game", nullptr);
		CheckFocus();
		if(*m_Playing)
		{
			ImGui::Text("Playing");
		}
		else
		{
			ImGui::Text("Not Playing");
		}
		ImGui::End();
	}

}