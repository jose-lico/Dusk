#include "InspectorPanel.h"

#include "DuskEngine.h"

#include "imgui.h"
#include "nfd.h"

namespace DuskEngine
{
	InspectorPanel::InspectorPanel()
	{
	}

	void InspectorPanel::OnImGuiRender()
	{
		ImGui::Begin("Inspector");
		if (m_Entity)
		{
			auto& transform = m_Entity->GetComponent<Transform>();
			ImGui::Text("Transform");
			ImGui::DragFloat3("Position", &transform.Position[0], .01f);
			glm::vec3 rotation = glm::degrees(transform.Rotation);
			if(ImGui::DragFloat3("Rotation", &rotation[0], 0.1f, 0.0f, 0.0f))
			transform.Rotation = glm::radians(rotation);
			ImGui::DragFloat3("Scale", &transform.Scale[0], .01f);
			ImGui::Separator();

			auto& mesh = m_Entity->GetComponent<MeshRenderer>();
			ImGui::Text("Mesh Renderer");
			if(ImGui::ImageButton((void*)mesh.TX->GetRendererID(), ImVec2{ 80, 80 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{

				APP_LOG("It clicked!")
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("png,jpg;pdf", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					APP_LOG("Success!");
					APP_LOG(outPath);
					mesh.TX.reset(Texture::Create(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
					APP_LOG("User pressed cancel.");
				}
				else
				{
					APP_LOG("Error: %s\n", NFD_GetError());
				}
			}
		}
		ImGui::End();
	}
}