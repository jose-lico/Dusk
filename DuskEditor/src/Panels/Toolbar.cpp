#include "pch.h"
#include "Toolbar.h"

#include "Core/Resources/Resources/Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace DuskEngine
{
	Toolbar::Toolbar(bool* playing, bool* paused, const std::function<void(void)>& playfunc, const std::function<void(void)>& stopfunc, const std::function<void(void)>& pausefunc)
		:m_Playing(playing), m_Paused(paused), m_PlayFunc(playfunc), m_StopFunc(stopfunc), m_PauseFunc(pausefunc)
	{
		m_Play  = Texture::Create("res/editor/icons/play.png");
		m_Stop  = Texture::Create("res/editor/icons/stop.png");
		m_Pause = Texture::Create("res/editor/icons/pause.png");
	}

	void Toolbar::OnImGuiRender()
	{
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeFlagsMask_;
		ImGui::SetNextWindowClass(&window_class);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
		ImGui::Begin("##Toolbar", nullptr, window_flags);
		CheckFocus();

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - ((26.0f * 2) * 0.5f));
		if (ImGui::ImageButton((ImTextureID)(!*m_Playing ? m_Play->GetRendererID() : m_Stop->GetRendererID()), ImVec2(18.0f,18.0f)))
		{
			if (!*m_Playing)
				(m_PlayFunc)();
			else 
				(m_StopFunc)();
		}

		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)(!*m_Paused ? m_Pause->GetRendererID() : m_Play->GetRendererID()), ImVec2(18.0f, 18.0f)))
		{
			if (*m_Playing)
				(m_PauseFunc)();
		}

		ImGui::End();
	}
}