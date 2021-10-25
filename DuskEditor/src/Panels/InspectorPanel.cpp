#include "InspectorPanel.h"

#include "imgui.h"

InspectorPanel::InspectorPanel()
{
}

void InspectorPanel::OnImGuiRender()
{
	if(m_Entity)
	{
		ImGui::Begin("Inspector");
		auto& transform = m_Entity->GetComponent<DuskEngine::Transform>();
		ImGui::DragFloat3("Position", &transform.Position[0], .01f);
		ImGui::End();
	}
}