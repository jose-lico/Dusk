#include "SceneViewportPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <IconsForkAwesome.h>

namespace DuskEngine
{
	SceneViewportPanel::SceneViewportPanel(Ref<FrameBuffer>& fb, Entity camera)
	{
		m_ViewportSize = glm::vec2(0.0f);
		m_FB = fb;
		m_Camera = camera;
	}

	static bool stats = false;
	void SceneViewportPanel::OnImGuiRender()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin(ICON_FK_EYE " Viewport", (bool *)true, window_flags);

		if (ImGui::BeginMenuBar())
		{
            ImGui::MenuItem("Stats", "", &stats);

			ImGui::EndMenuBar();
		}

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportSize)
		{
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_FB->Resize(m_ViewportSize);
			m_Camera.GetComponent<Camera>().projectionMatrix = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.01f, 100.0f);
		}
		ImGui::Image((void*)m_FB->GetColorAttachmentID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::PopStyleVar();
        ImVec2 work_pos = ImGui::GetWindowPos();
        auto viewportID = ImGui::GetWindowViewport()->ID;

        if (stats && !ImGui::IsWindowCollapsed())
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings 
                | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove;
    
            ImVec2 window_pos = { work_pos.x + 10 , work_pos.y + 50 };
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2());

			ImGui::SetNextWindowViewport(viewportID);
            ImGui::SetNextWindowBgAlpha(0.35f);
            if (ImGui::Begin("Example: Simple overlay", &stats, window_flags))
            {
                ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
                ImGui::Separator();
            }
            ImGui::End();
        }

		ImGui::End();
	}
}