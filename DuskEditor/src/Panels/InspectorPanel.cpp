#include "InspectorPanel.h"

#include "DuskEngine.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <nfd/nfd.h>
#include <IconsForkAwesome.h>

namespace DuskEngine
{
	template<typename T, typename UIFunction>
	static void DrawComponent(const char* name, std::vector<Entity*>& m_SelectedEntities, UIFunction function);

	InspectorPanel::InspectorPanel()
	{
	}

	void InspectorPanel::OnImGuiRender()
	{
		ImGui::Begin(ICON_FK_INFO_CIRCLE " Inspector");

		if(m_SelectedEntities->size() > 0)
		{
			if (m_SelectedEntities->size() > 1)
			{
				ImGui::Text("Inspecting multiple objects.");
				ImGui::Text("Only shared components can be edited.");
				ImGui::Separator();
			}

			DrawComponent<Transform>(ICON_FK_CUBE " Transform", *m_SelectedEntities, TransformInspector);
			DrawComponent<Camera>(ICON_FK_VIDEO_CAMERA " Camera", *m_SelectedEntities, CameraInspector);
			DrawComponent<Light>(ICON_FK_LIGHTBULB_O " Light", *m_SelectedEntities, LightInspector);
			DrawComponent<MeshRenderer>(ICON_FK_PAINT_BRUSH " Material", *m_SelectedEntities, MaterialInspector);
		}
		ImGui::End();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const char* name, std::vector<Entity*>& m_SelectedEntities, UIFunction function)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		std::vector<T*> components;

		for (auto ent : m_SelectedEntities)
		{
			if (!ent->HasComponent<T>())
				return;

			components.push_back(&ent->GetComponent<T>());
		}

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name);
		ImGui::PopStyleVar(
		);
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			function(components);
			ImGui::TreePop();
		}

		if (removeComponent)
		{
			for (auto ent : m_SelectedEntities)
				ent->RemoveComponent<T>();
		}
			
	}

	void InspectorPanel::CameraInspector(std::vector<Camera*>& cameras)
	{
		if (cameras.size() == 1)
		{
			ImGui::Checkbox("Primary Camera", &(cameras[0]->main));

			// Add projection type
		}
		else
		{
			// TODO
		}
	}
	
	void InspectorPanel::TransformInspector(std::vector<Transform*>& transforms)
	{
		if(transforms.size() == 1)
		{
			ImGui::DragFloat3("Position", &(transforms[0]->position[0]), .01f);

			glm::vec3 rotation = glm::degrees(transforms[0]->rotation);
			if (ImGui::DragFloat3("Rotation", &rotation[0], 0.1f, 0.0f, 0.0f))
				transforms[0]->rotation = glm::radians(rotation);

			ImGui::DragFloat3("Scale", &(transforms[0]->scale[0]), .01f);
		}
		else
		{
			glm::vec3 translation(0.0f);
			ImGui::DragFloat3("Position", &translation[0], .01f);

			glm::vec3 rotation(0.0f);
			ImGui::DragFloat3("Rotation", &rotation[0], .01f);

			glm::vec3 scale(0.0f);
			ImGui::DragFloat3("Scale", &scale[0], .01f);

			for(auto& t : transforms)
			{
				t->position += translation;
				t->rotation += rotation;
				t->scale += scale;
			}
		}
	}

	void InspectorPanel::LightInspector(std::vector<Light*>& lights)
	{
		const char* types[] = { "Directional", "Point" };
		static int item_current_idx = 0;
		const char* combo_label = types[item_current_idx];

		if (lights.size() == 1)
		{
			ImGui::ColorEdit3("Light Color", &(lights[0]->color[0]));

			if (ImGui::BeginCombo("Type", combo_label))
			{
				for (int n = 0; n < IM_ARRAYSIZE(types); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(types[n], is_selected))
					{
						if (n != item_current_idx)
						{
							item_current_idx = n;
							if (types[n] == "Directional")
								lights[0]->type = LightType::Directional;
							if (types[n] == "Point")
								lights[0]->type = LightType::Point;
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		else
		{
			// TODO - Selecting multiple lights will change both colors and types instantly
			glm::vec3 color(1.0f);
			ImGui::ColorEdit3("Light Color", &color[0]);

			LightType type = LightType::Directional;

			if (ImGui::BeginCombo("Type", combo_label))
			{
				for (int n = 0; n < IM_ARRAYSIZE(types); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(types[n], is_selected))
					{
						if (n != item_current_idx)
						{
							item_current_idx = n;
							if (types[n] == "Directional")
								type = LightType::Directional;
							if (types[n] == "Point")
								type = LightType::Point;
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			for (auto& l : lights)
			{
				l->type = type;
				l->color = color;
			}
		}
	}

	void InspectorPanel::MaterialInspector(std::vector<MeshRenderer*>& meshes)
	{
		if (meshes.size() == 1)
		{
			ImGui::Text(meshes[0]->material->GetName().c_str());

			const char* shaders[] = { "phong.glsl", "simpleColor.glsl", "simpleTexture.glsl" };
			static int item_current_idx = 0;
			const char* combo_label = shaders[item_current_idx];
			if (ImGui::BeginCombo("Shader", combo_label))
			{
				for (int n = 0; n < IM_ARRAYSIZE(shaders); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(shaders[n], is_selected))
					{
						if (n != item_current_idx)
						{
							item_current_idx = n;
							std::string s = shaders[n];
							meshes[0]->material = MakeRef<Material>(Shader::Create("res/shaders/" + s));
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			for (auto uniform : meshes[0]->material->m_Uniforms)
			{
				if (uniform.Type == UniformType::Vec3)
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
							// very temporary, but works!
							auto texture = Texture::Create(path);
							std::stringstream ss;
							ss << path;
							ss << ".meta";
							texture->m_UUID = *uuids::uuid::from_string(ResourceManager::GetUUID(ss.str()));
							meshes[0]->material->SetTexture(uniform.Name,texture);
							meshes[0]->material->SerializeText("res/materials/cubeMaterial.material");
							free(path);
						}
					}
				}
			}
		}
		else
		{
			// TODO only same material instance can be edited.
		}
	}
}