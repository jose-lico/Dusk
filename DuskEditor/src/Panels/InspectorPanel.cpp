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
				ImGui::DragFloat3("Position", &transform.position[0], .01f);

				glm::vec3 rotation = glm::degrees(transform.rotation);
				if (ImGui::DragFloat3("Rotation", &rotation[0], 0.1f, 0.0f, 0.0f))
					transform.rotation = glm::radians(rotation);
				
				ImGui::DragFloat3("Scale", &transform.scale[0], .01f);
				ImGui::Separator();
			}
			
			if (m_Entity->HasComponent<MeshRenderer>())
			{
				auto& mesh = m_Entity->GetComponent<MeshRenderer>();
				
				ImGui::Text("Mesh Renderer");
				ImGui::Text("Mesh");
				
				ImGui::Separator();

				ImGui::Text("Material");
				for(auto uniform : mesh.material->m_Uniforms)
				{
					if(uniform.Type == UniformType::Vec3)
					{
						ImGui::ColorEdit3(uniform.Name.c_str(), std::static_pointer_cast<float>(uniform.Data).get());
					}

					if (uniform.Type == UniformType::Texture)
					{
						ImGui::Text(uniform.Name.c_str());
						if (ImGui::ImageButton((void*)std::static_pointer_cast<Texture>(uniform.Data)->GetRendererID(), ImVec2{ 40, 40 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
						{
							nfdchar_t* path = NULL;
							nfdresult_t result = NFD_OpenDialog("png;jpg", NULL, &path);
							if (result == NFD_OKAY)
							{
								mesh.material->SetTexture(uniform.Name, Texture::Create(path));
								free(path);
							}
						}
					}
				}
				ImGui::Separator();
			}

			if (m_Entity->HasComponent<Camera>())
			{
				auto& camera = m_Entity->GetComponent<Camera>();
				ImGui::Text("Camera");
				ImGui::Checkbox("Primary Camera", &camera.main);

				ImGui::Separator();
			}
		}
		ImGui::End();
	}
}