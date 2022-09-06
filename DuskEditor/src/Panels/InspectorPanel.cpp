#include "InspectorPanel.h"

#include "Core/Application/Application.h"
#include "Core/Assets/AssetDatabase.h"
#include "Core/Assets/AssetHandler.h"
#include "Platform/OpenGL/Shader.h"
#include "Core/Assets/Assets/Mesh.h"
#include "Core/Assets/Assets/Material.h"
#include "Platform/OpenGL/Texture.h"
#include "Core/Scripting/LuaScript.h"
#include "Core/ECS/Entity.h"

#include "Utils/ImGuiUtils.h"
#include "Utils/Rendering/PrimitiveMesh.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "IconsForkAwesome.h"

namespace DuskEngine
{
	template<typename T, typename UIFunction>
	static void DrawComponentStatic(const char* name, std::vector<Entity>& m_SelectedEntities, UIFunction function);

	template<typename T, typename Arg, typename TOwner, void(TOwner::* func)(Arg a)>
	void DrawComponent(const char* name, std::vector<Entity>& m_SelectedEntities, TOwner* p);

	InspectorPanel::InspectorPanel(AssetHandler* assetHandler)
		:m_AssetHandler(assetHandler)
	{
		m_DB = &Application::Get().GetAssetDatabase();
	}

	void InspectorPanel::OnImGuiRender()
	{
		ImGui::Begin(ICON_FK_INFO "  Inspector");
		CheckFocus();

		if(m_SelectedEntities->size() > 0)
		{
			if (m_SelectedEntities->size() > 1)
			{
				ImGui::Text("Inspecting multiple objects.");
				ImGui::Text("Only shared components can be edited.");
				ImGui::Separator();
			}
			else
			{
				if (ImGui::Button("Add Component"))
					ImGui::OpenPopup("AddComponent");

				if (ImGui::BeginPopup("AddComponent"))
				{
					if (ImGui::MenuItem("Camera"))
					{
						auto& camera = (*m_SelectedEntities)[0].AddComponent<Camera>();
						camera.projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f);
						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItem("Light"))
					{
						(*m_SelectedEntities)[0].AddComponent<Light>();
						ImGui::CloseCurrentPopup();
					}

					if(ImGui::MenuItem("Mesh Renderer"))
					{
						auto& mesh = (*m_SelectedEntities)[0].AddComponent<MeshRenderer>();
						mesh.meshHandle = 1;
						mesh.materialHandle = 0;
						//mesh.mesh = PrimitiveMesh::Quad();
						// This will later be changed to a default material
						//mesh.material = ResourceManager::LoadMaterial(ResourceManager::GetUUID("res/materials/modelMaterial.material"));
						ImGui::CloseCurrentPopup();
					}

					if (ImGui::BeginMenu("Scripts"))
					{
						for (auto scriptAsset : m_DB->ScriptsDatabase)
						{
							if(ImGui::MenuItem(scriptAsset->GetName().c_str()))
							{
								//auto& script = (*m_SelectedEntities)[0].AddComponent<Script>();
								//script.luaScriptHandle = scriptAsset->GetUUID();
								//m_AssetHandler->AddToLuaScriptPool((*m_SelectedEntities)[0].GetComponent<Meta>().entityHandle, script.luaScriptHandle);
							}
						}
						ImGui::EndMenu();
					}

					ImGui::EndPopup();
				}
			}

			DrawComponentStatic<Meta>(ICON_FK_CUBE "  Meta", *m_SelectedEntities, MetaInspector);
			DrawComponentStatic<Transform>(ICON_FK_ARROWS_ALT "  Transform", *m_SelectedEntities, TransformInspector);
			DrawComponentStatic<Camera>(ICON_FK_VIDEO_CAMERA "  Camera", *m_SelectedEntities, CameraInspector);
			DrawComponentStatic<Light>(ICON_FK_LIGHTBULB_O "  Light", *m_SelectedEntities, LightInspector);
			DrawComponent<MeshRenderer, std::vector<MeshRenderer*>& ,InspectorPanel, &InspectorPanel::MaterialInspector>(ICON_FK_PAINT_BRUSH "  Mesh Renderer", *m_SelectedEntities, this);

			/*std::string script = ICON_FK_PENCIL_SQUARE_O "  Lua Script - ";
			script.append(m_AssetHandler->LuaScriptPool((*m_SelectedEntities)[0].GetComponent<Meta>().entityHandle)->GetName());*/
			DrawComponent<Script, std::vector<Script*>&, InspectorPanel, &InspectorPanel::ScriptInspector>
				(ICON_FK_PENCIL_SQUARE_O "  Lua Script", *m_SelectedEntities, this);
		}
		ImGui::End();
	}

	template<typename T, typename UIFunction>
	static void DrawComponentStatic(const char* name, std::vector<Entity>& m_SelectedEntities, UIFunction function)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		std::vector<T*> components;

		for (auto ent : m_SelectedEntities)
		{
			if (!ent.HasComponent<T>())
				return;

			components.push_back(&ent.GetComponent<T>());
		}

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];
		ImGui::PushFont(boldFont);
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name);
		ImGui::PopStyleVar();
		ImGui::PopFont();

		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button(ICON_FK_COG, ImVec2{ lineHeight, lineHeight }))
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
				ent.RemoveComponent<T>();
		}
			
	}

	template<typename T, typename Arg, typename TOwner, void(TOwner::* func)(Arg a)>
	void DrawComponent(const char* name, std::vector<Entity>& m_SelectedEntities, TOwner* p)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		std::vector<T*> components;

		for (auto ent : m_SelectedEntities)
		{
			if (!ent.HasComponent<T>())
				return;

			components.push_back(&ent.GetComponent<T>());
		}

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];
		ImGui::PushFont(boldFont);
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name);
		ImGui::PopStyleVar();
		ImGui::PopFont();

		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button(ICON_FK_COG, ImVec2{ lineHeight, lineHeight }))
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
			(p->*func)(components);
			ImGui::TreePop();
		}

		if (removeComponent)
		{
			for (auto ent : m_SelectedEntities)
				ent.RemoveComponent<T>();
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
	
	void InspectorPanel::MetaInspector(std::vector<Meta*>& metas)
	{
		if (metas.size() == 1)
		{
			char buffer[64];
			sprintf_s(buffer, metas[0]->name.c_str());
			if(ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
			{
				metas[0]->name = buffer;
			}

			ImGui::Checkbox("Enabled", &metas[0]->enabled);
		}
		else
		{
			bool enabled = metas[0]->enabled;
			if(ImGui::Checkbox("Enabled", &enabled))
				for (auto& m : metas)
					m->enabled = enabled;
		}
	}

	void InspectorPanel::TransformInspector(std::vector<Transform*>& transforms)
	{
		if(transforms.size() == 1)
		{
			//static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable;
			////static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
			//ImGui::BeginTable("table2", 2, flags);

			//ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthStretch);
			//ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthFixed);
			//
			//ImGuiUtils::DrawVec3Table("Position", transforms[0]->position);
			//ImGuiUtils::DrawVec3Table("Scale", transforms[0]->scale);
			//ImGuiUtils::DrawFloatTable("Rotation", transforms[0]->rotation.x);
			//ImGuiUtils::DrawTextTable("Property");

			//ImGui::EndTable();

			ImGui::DragFloat3("Position", &(transforms[0]->position[0]), .01f);

			glm::vec3 rotation = glm::degrees(transforms[0]->rotation);
			if (ImGui::DragFloat3("Rotation", &rotation[0], 0.1f, 0.0f, 0.0f))
				transforms[0]->rotation = glm::radians(rotation);

			ImGui::DragFloat3("Scale", &(transforms[0]->scale[0]), .01f);
			
			// for debug purposes
			//ImGui::DragFloat3("Front", &(transforms[0]->front[0]), .01f);
			/*ImGui::DragFloat4("##Mat1", &(transforms[0]->GetTransform()[0][0]), .01f);
			ImGui::DragFloat4("##Mat2", &(transforms[0]->GetTransform()[1][0]), .01f);
			ImGui::DragFloat4("##Mat3", &(transforms[0]->GetTransform()[2][0]), .01f);
			ImGui::DragFloat4("##Mat4", &(transforms[0]->GetTransform()[3][0]), .01f);*/
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
			/*glm::vec3 color(1.0f);
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
			}*/
		}
	}

	void InspectorPanel::MaterialInspector(std::vector<MeshRenderer*>& meshes)
	{
		if (meshes.size() == 1)
		{
			// This could and should probably be done once at startup, and refreshed once a new shader is added/deleted
			std::vector<std::string> modelList {"Primitive: Quad", "Primitive: Cube"};
			int modelIndex = 0;
			uuids::uuid modelID = m_AssetHandler->AssetPool<Mesh>(meshes[0]->meshHandle)->GetUUID();

			for (unsigned int i = 0; i < m_DB->ModelDatabase.size(); i++)
			{
				modelList.push_back(m_DB->ModelDatabase[i]->GetName());
				if (m_DB->ModelDatabase[i]->GetUUID() == modelID)
					modelIndex = i + 2;
			}

			if(!modelIndex)
			{
				switch (m_AssetHandler->AssetPool<Mesh>(meshes[0]->meshHandle)->GetType())
				{
				case MeshType::Quad:
					modelIndex = 0;
					break;
				case MeshType::Cube:
					modelIndex = 1;
					break;
				default:
					break;
				}
			}

			const char* model_label = modelList[modelIndex].c_str();
			if (ImGui::BeginCombo("Mesh", model_label))
			{
				for (int n = 0; n < modelList.size(); n++)
				{
					const bool is_selected = (modelIndex == n);
					if (ImGui::Selectable(modelList[n].c_str(), is_selected))
					{
						if (n != modelIndex)
						{
							modelIndex = n;
							if(modelIndex <= 1)
							{
								switch (modelIndex)
								{
								case 0:
									meshes[0]->meshHandle = 0;
									break;
								case 1:
									meshes[0]->meshHandle = 1;
									break;
								default:
									break;
								}
							}
							else
							{	
								meshes[0]->meshHandle = 
									m_AssetHandler->AddToAssetPool<Mesh>(m_DB->GetUUID(m_DB->ModelDatabase[modelIndex - 2]->GetPath()));
							}
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// This could and should probably be done once at startup, and refreshed once a new shader is added/deleted
			std::vector<std::string> materialList; // Later add default materials
			int materialIndex = 0;
			uuids::uuid materialID = m_AssetHandler->AssetPool<Material>(meshes[0]->materialHandle)->GetUUID();
				;

			for (unsigned int i = 0; i < m_DB->MaterialDatabase.size(); i++)
			{
				materialList.push_back(m_DB->MaterialDatabase[i]->GetName());
				if (m_DB->MaterialDatabase[i]->GetUUID() == materialID)
					materialIndex = i;
			}

			
			const char* material_label = materialList[materialIndex].c_str();
			if (ImGui::BeginCombo("Material", material_label))
			{
				for (int n = 0; n < materialList.size(); n++)
				{
					const bool is_selected = (materialIndex == n);
					if (ImGui::Selectable(materialList[n].c_str(), is_selected))
					{
						if (n != materialIndex)
						{
							materialIndex = n;
							auto id = m_DB->GetUUID(m_DB->MaterialDatabase[materialIndex]->GetPath());
							meshes[0]->materialHandle = m_AssetHandler->AddToAssetPool<Material>(id);
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();
			ImGui::Separator();
			ImGui::Separator();

			ImGui::Text(m_AssetHandler->AssetPool<Material>(meshes[0]->materialHandle)->GetName().c_str());

			// This could and should probably be done once at startup, and refreshed once a new shader is added/deleted
			std::vector<std::string> shaderList;
			int shaderIndex = 0;
			// bad
			uuids::uuid shaderID = m_AssetHandler->AssetPool<Shader>(
				m_AssetHandler->AssetPool<Material>(meshes[0]->materialHandle)->GetShaderHandle())->GetUUID();

			for (unsigned int i = 0; i < m_DB->ShaderDatabase.size(); i++)
			{
				shaderList.push_back(m_DB->ShaderDatabase[i]->GetName());
				if (m_DB->ShaderDatabase[i]->GetUUID() == shaderID)
					shaderIndex = i;
			}

			const char* shader_label = shaderList[shaderIndex].c_str();
			if (ImGui::BeginCombo("Shader", shader_label))
			{
				for (int n = 0; n < shaderList.size(); n++)
				{
					const bool is_selected = (shaderIndex == n);
					if (ImGui::Selectable(shaderList[n].c_str(), is_selected))
					{
						if (n != shaderIndex)
						{
							shaderIndex = n;
							
							auto id = m_DB->GetUUID(m_DB->ShaderDatabase[shaderIndex]->GetPath());
							auto shaderHandle = m_AssetHandler->AddToAssetPool<Shader>(id);
							m_AssetHandler->AssetPool<Material>(meshes[0]->materialHandle)->SetShader(shaderHandle);
							
							m_AssetHandler->AssetPool<Material>(meshes[0]->materialHandle)->SerializeText(
								m_AssetHandler->AssetPool<Material>(meshes[0]->materialHandle)->GetPath().string());
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// actually this logic is now kinda sus but might be relevant again in the future
			
			//// if in editing mode ->					propagate
			//// if accessing from asset browser ->		propagate
			//// if playing and accessing from scene ->	do not propagate

			auto& material = m_AssetHandler->AssetPool<Material>(meshes[0]->materialHandle);

			for (auto& uniform : m_AssetHandler->AssetPool<Material>(meshes[0]->materialHandle)->m_Uniforms)
			{
				if (uniform.Type == UniformType::Vec3)
				{
					if (ImGui::ColorEdit3(uniform.Name.c_str(), &uniform.Data.vec3[0]))
						material->SerializeText(material->GetPath().string());
				}

				if (uniform.Type == UniformType::Texture)
				{
					// this is broken but whatever, will be replaced in the future
					ImGui::Text(uniform.Name.c_str());
					if (ImGui::ImageButton((ImTextureID)(size_t)m_AssetHandler->AssetPool<Texture>(uniform.Data.dataHandle)->GetRendererID(),
						ImVec2{40, 40}, ImVec2{0, 1}, ImVec2{1, 0}))
					{
						TRACE("To be implemented");
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
						{
							const wchar_t* data = (const wchar_t*)payload->Data;
							std::wstring ws(data);

							auto& texture = m_AssetHandler->AssetPool<Texture>(m_AssetHandler->AddToAssetPool<Texture>(m_DB->GetUUID(ws)));
							material->SetTexture(uniform.Name, texture);
							material->SerializeText(material->GetPath().string());
						}
						ImGui::EndDragDropTarget();
					}
				}
			}
		}
		else
		{
			// TODO only same material instance can be edited.
		}
	}

	void InspectorPanel::ScriptInspector(std::vector<Script*>& scripts)
	{
		if (scripts.size() == 1)
		{	
			//ImGui::Text(m_AssetHandler->LuaScriptPool()->GetName().c_str();
			
			//for (unsigned int i = 0; i < scripts[0]->scripts.size(); i++)
			//{
			//	ImGui::Text(scripts[0]->scripts[i]->GetName().c_str());
			//	ImGui::SameLine();
			//	if (ImGui::Button(("Remove script " + std::to_string(i)).c_str()))
			//	{
			//		scripts[0]->scripts.erase(scripts[0]->scripts.begin() + i--);
			//	}
			//}

			//if(scripts[0]->scripts.size() == 0)
			//{
			//	// Remove component, but cant be done with current setup. inspector needs rework anyway so w/e
			//}
		}
	}
}