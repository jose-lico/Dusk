#include "pch.h"
#include "Toolbar.h"

#include "Platform/OpenGL/Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "IconsForkAwesome.h"

namespace DuskEngine
{
	Toolbar::Toolbar(bool* playing, bool* paused, const std::function<void(void)>& playfunc, const std::function<void(void)>& stopfunc, const std::function<void(void)>& pausefunc)
		:m_Playing(playing), m_Paused(paused), m_PlayFunc(playfunc), m_StopFunc(stopfunc), m_PauseFunc(pausefunc)
	{
		m_Play  = MakeRef<Texture>("res/editor/icons/play.png");
		m_Stop  = MakeRef<Texture>("res/editor/icons/stop.png");
		m_Pause = MakeRef<Texture>("res/editor/icons/pause.png");
		m_StepForward = MakeRef<Texture>("res/editor/icons/step-forward.png");
		m_Reload = MakeRef<Texture>("res/editor/icons/reload.png");
	}

	void Toolbar::OnImGuiRender()
	{
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeFlagsMask_;
		ImGui::SetNextWindowClass(&window_class);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;

		//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
		//ImGui::Begin("##Toolbar", nullptr, window_flags);
		//CheckFocus();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4,8});
		//ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0);
		
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { m_AlphasMenus[0], m_AlphasMenus[0], m_AlphasMenus[0], 1.0f });

			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New Scene", "Ctrl + N");
				ImGui::MenuItem("Open Scene", "Ctrl + O");
				ImGui::Separator();
				ImGui::MenuItem("Save", "Ctrl + S");
				ImGui::MenuItem("Save as", "Ctrl + Shift + S");
				ImGui::Separator();
				ImGui::MenuItem("New Project");
				ImGui::MenuItem("Open Project");

				ImGui::EndMenu();
			}
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
				m_AlphasMenus[0] = 1.0f;
			else
				m_AlphasMenus[0] = 0.87f;

			ImGui::PushStyleColor(ImGuiCol_Text, { m_AlphasMenus[1], m_AlphasMenus[1], m_AlphasMenus[1], 1.0f });

			if (ImGui::BeginMenu("Project"))
			{
				ImGui::MenuItem("Project Settings");
				ImGui::Separator();
				ImGui::MenuItem("Export");

				ImGui::EndMenu();
			}
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
				m_AlphasMenus[1] = 1.0f;
			else
				m_AlphasMenus[1] = 0.87f;

			ImGui::PushStyleColor(ImGuiCol_Text, { m_AlphasMenus[2], m_AlphasMenus[2], m_AlphasMenus[2], 1.0f });
			if (ImGui::BeginMenu("Editor"))
			{
				ImGui::MenuItem("Editor Settings");
				ImGui::MenuItem("Layout");

				ImGui::EndMenu();
			}
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
				m_AlphasMenus[2] = 1.0f;
			else
				m_AlphasMenus[2] = 0.87f;

			ImGui::PushStyleColor(ImGuiCol_Text, { m_AlphasMenus[3], m_AlphasMenus[3], m_AlphasMenus[3], 1.0f });
			if (ImGui::BeginMenu("About"))
			{
				ImGui::MenuItem("Github");

				ImGui::EndMenu();
			}
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
				m_AlphasMenus[3] = 1.0f;
			else
				m_AlphasMenus[3] = 0.87f;

			ImGui::SameLine();

			ImVec2 buttonSize = ImVec2(18.0f, 18.0f);
			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - 128);
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

			if (ImGui::ImageButton((ImTextureID)(size_t)
				(!*m_Playing ? m_Play->GetRendererID() : m_Stop->GetRendererID()), buttonSize, { 0,0 }, { 1,1 }, -1, { 0,0,0,0 },
				{ 1.0f, 1.0f, 1.0f, m_AlphasButtons[0] }))
			{
				if (!*m_Playing)
					(m_PlayFunc)();
				else
					(m_StopFunc)();
			}

			if (ImGui::IsItemHovered())
				m_AlphasButtons[0] = 1.0f;
			else
				m_AlphasButtons[0] = 0.87f;

			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)(size_t)(!*m_Paused ? m_Pause->GetRendererID() : m_Play->GetRendererID()), buttonSize,
				{ 0,0 }, { 1,1 }, -1, { 0,0,0,0 }, { 1.0f, 1.0f, 1.0f, m_AlphasButtons[1] }))
			{
				if (*m_Playing)
					(m_PauseFunc)();
			}

			if (*m_Playing)
			{
				if (ImGui::IsItemHovered())
					m_AlphasButtons[1] = 1.0f;
				else
					m_AlphasButtons[1] = 0.87f;
			}
			else
				m_AlphasButtons[1] = 0.4f;


			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)(size_t)m_StepForward->GetRendererID(), buttonSize,
				{ 0,0 }, { 1,1 }, -1, { 0,0,0,0 }, { 1.0f, 1.0f, 1.0f, m_AlphasButtons[2] }))
			{
				WARN("Step forward not implemented!");
			}

			if (*m_Playing && *m_Paused)
			{
				if (ImGui::IsItemHovered())
					m_AlphasButtons[2] = 1.0f;
				else
					m_AlphasButtons[2] = 0.87f;
			}
			else
				m_AlphasButtons[2] = 0.4f;

			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)(size_t)m_Reload->GetRendererID(), buttonSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 },
				-1, { 0,0,0,0 }, { 1.0f, 1.0f, 1.0f, m_AlphasButtons[3] }))
			{
				WARN("Reloading scripts not implemented!");
			}

			if (*m_Playing)
			{
				if (ImGui::IsItemHovered())
					m_AlphasButtons[3] = 1.0f;
				else
					m_AlphasButtons[3] = 0.87f;
			}
			else
				m_AlphasButtons[3] = 0.4f;

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::EndMainMenuBar();
		}
		ImGui::PopStyleVar();
		//ImGui::PopStyleVar();
		//ImGui::End();
		//ImGui::PopStyleColor();
	}
}