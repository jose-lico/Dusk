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
			if(m_Entity->HasComponent<Transform>())
			{
				auto& transform = m_Entity->GetComponent<Transform>();
				ImGui::Text("Transform");
				ImGui::DragFloat3("Position", &transform.Position[0], .01f);

				glm::vec3 rotation = glm::degrees(transform.Rotation);
				if (ImGui::DragFloat3("Rotation", &rotation[0], 0.1f, 0.0f, 0.0f))
					transform.Rotation = glm::radians(rotation);
				
				ImGui::DragFloat3("Scale", &transform.Scale[0], .01f);
				ImGui::Separator();
			}
			
			if (m_Entity->HasComponent<MeshRenderer>())
			{
				auto& mesh = m_Entity->GetComponent<MeshRenderer>();
				ImGui::Text("Mesh Renderer");
				if (ImGui::ImageButton((void*)mesh.TX->GetRendererID(), ImVec2{ 80, 80 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
				{
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("png,jpg;pdf", NULL, &outPath);
					if (result == NFD_OKAY)
					{
						mesh.TX.reset(Texture::Create(outPath));
						free(outPath);
					}
				}

				if(mesh.MaterialTeste)
				{
					for(auto uniform : mesh.MaterialTeste->m_Uniforms)
					{
						if(uniform.Type == UniformType::Vec3)
						{
							ImGui::ColorEdit3(uniform.Name.c_str(), (float*)uniform.Data);
						}
					}
				}
				ImGui::Separator();
			}

			if (m_Entity->HasComponent<Camera>())
			{
				auto& camera = m_Entity->GetComponent<Camera>();
				ImGui::Text("Camera");
				ImGui::Checkbox("Primary Camera", &camera.MainCamera);

				ImGui::Separator();
			}
		}
		ImGui::End();
	}
}