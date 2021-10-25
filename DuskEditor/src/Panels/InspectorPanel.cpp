#include "InspectorPanel.h"

#include "imgui.h"

namespace DuskEngine
{
	InspectorPanel::InspectorPanel()
	{
	}

	void InspectorPanel::OnImGuiRender()
	{
		if (m_Entity)
		{
			ImGui::Begin("Inspector");
			auto& transform = m_Entity->GetComponent<Transform>();
			ImGui::DragFloat3("Position", &transform.Position[0], .01f);
			ImGui::End();
		}
	}
}