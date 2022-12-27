#include "ContentBrowserPanel.h"

#include "Core/Application/Core.h"
#include "Core/Application/Application.h"
#include "Core/Assets/AssetDatabase.h"
#include "Core/Assets/Assets/Material.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Utils/Profiling/Timer.h"

#include "images/UnknownIcon.png.embedded"
#include "images/FolderIcon.png.embedded"

#include "IconsForkAwesome.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

namespace DuskEngine
{
	std::filesystem::path* g_currentDir;

	ContentBrowserPanel::ContentBrowserPanel(const std::string& projectPath)
	{
		m_FolderIcon = CreateTexture(EMBEDDED_FOLDERICON);
		m_UnknownIcon = CreateTexture(EMBEDDED_UNKNOWNICON);
 
		m_RootDirectory = projectPath;
		g_currentDir = &m_CurrentDirectory;
		m_CurrentDirectory = m_RootDirectory;

		//m_Icons.resize(50);

		CreateDirectoryItems();
		CreateDirectoryResources();
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
		OpenGLAPI::FreeTexture(m_FolderIcon);
		OpenGLAPI::FreeTexture(m_UnknownIcon);
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin(ICON_FK_FOLDER_OPEN "  Asset Browser");
		CheckFocus();

		if (m_CurrentDirectory != std::filesystem::path(m_RootDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
				CreateDirectoryItems();
				CreateDirectoryResources();
			}
		}

		ImGuiStyle& style = ImGui::GetStyle();

		uint32_t buttonsCount = (uint32_t)m_DirEntries.size();
		uint32_t textureIndex = 0;
		float windowVisibleX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		float buttonSize = 64.0f;
		float buttonSpacing = 25.0f;
		bool shouldBreak = false;

		for (size_t i = 0; i < buttonsCount; i++)
		{
			auto& directoryEntry = m_DirEntries[i];

			ImGui::BeginGroup();

			ImGui::PushID(i);
			if (directoryEntry.is_directory())
			{
				if (ImGui::ImageButton((ImTextureID)(uint64_t)m_FolderIcon.ResourceID, ImVec2(buttonSize, buttonSize), ImVec2(0, 1), ImVec2(1, 0))) 
				{
					m_CurrentDirectory /= directoryEntry.path().filename();
					CreateDirectoryItems();
					CreateDirectoryResources();
					shouldBreak = true;
				}
			}
			else if (directoryEntry.path().extension() == ".png" || directoryEntry.path().extension() == ".jpg")
			{
				ImGui::ImageButton((ImTextureID)(uint64_t)m_Icons[textureIndex++].ResourceID, ImVec2(buttonSize, buttonSize), ImVec2(0, 1), ImVec2(1, 0));
			}
			else
			{
				ImGui::ImageButton((ImTextureID)(uint64_t)m_UnknownIcon.ResourceID, ImVec2(buttonSize, buttonSize), ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::PopID();

			float lastButtonX = ImGui::GetItemRectMax().x;
			float nextButtonX = lastButtonX + style.ItemSpacing.x + buttonSize + buttonSpacing * 2.0f;

			ImGui::Text("%.12s", directoryEntry.path().filename().string().c_str());
			ImGui::EndGroup();

			if (i + 1 < buttonsCount && nextButtonX < windowVisibleX)
				ImGui::SameLine(0.0f, buttonSpacing);

			if (shouldBreak) break; // Only break here to avoid breaking ImGui state
		}

//
//		int buttons_count = (int)m_DirEntries.size();
//		ImGuiStyle& style = ImGui::GetStyle();
//
//		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
//
//		int i = 0;
//		int textureIndex = 0;
//
//		bool shouldBreak = false;
//
//		for(auto& directoryEntry : m_DirEntries)
//		{
//			const auto& path = directoryEntry.path();
//			std::string filenameString = path.stem().string();
//			auto extension = directoryEntry.path().extension();
//			if (extension == ".meta")
//				continue;
//
//			ImGui::PushID(filenameString.c_str());
//
//			ImGui::BeginGroup();
//			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,0 });
//
//			if (directoryEntry.is_directory())
//			{
//				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
//				ImGui::ImageButton((ImTextureID)(size_t)m_FolderIcon.GetRendererID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
//				ImGui::PopStyleColor();
//
//				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
//				{
//					m_CurrentDirectory /= path.filename();
//					CreateDirectoryItems();
//					CreateDirectoryResources();
//					shouldBreak = true;
//				}
//			}
//			else if (extension == ".png" || extension == ".jpg")
//			{
//				ImGui::ImageButton((ImTextureID)(size_t)m_Icons[textureIndex++]->GetRendererID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
//				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
//				{
//					m_EditingName = i;
//					m_EditingPath = directoryEntry.path();
//				}	
//
//#ifdef DUSK_WINDOWS
//				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
//				{
//					const wchar_t* itemPath = path.c_str();
//					ImGui::SetDragDropPayload("TEXTURE", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
//					ImGui::EndDragDropSource();
//				}
//#endif
//			}
//			else
//			{
//				ImGui::ImageButton((ImTextureID*)(size_t)m_UnknownIcon.GetRendererID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
//				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
//				{
//					LOG("This is an image");
//					m_EditingName = i;
//					m_EditingPath = directoryEntry.path();
//				}
//			}
//
//			ImGui::PopStyleVar();
//
//			float last_button_x2 = ImGui::GetItemRectMax().x;
//			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 64 + 40;
//
//			ImGui::AlignTextToFramePadding();
//
//			if(i == m_EditingName)
//			{
//				char buffer[64];
//				if (ImGui::InputText("##itemName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) 
//				{
//					LOG(buffer);
//					LOG(m_EditingPath.string().c_str());
//					LOG(m_CurrentDirectory.string().c_str());
//					std::string targetPath = m_CurrentDirectory.string() + "\\" + buffer + m_EditingPath.extension().string();
//					std::filesystem::rename(m_EditingPath, targetPath);
//					std::filesystem::rename(m_EditingPath.string() + ".meta", targetPath + ".meta");
//					//AssetDatabase::LoadUUIDs();
//				}
//			}
//			else
//				ImGui::Text(filenameString.c_str());
//
//			ImGui::EndGroup();
//
//			if (i++ + 1 < buttons_count && next_button_x2 < window_visible_x2)
//				ImGui::SameLine(0.0f, 20.0f);
//
//			ImGui::PopID();
//
//			if (shouldBreak) break; // Only break here to avoid breaking ImGui
//		}
//
//		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
//			m_EditingName = -1;
//
//		if (ImGui::BeginPopupContextWindow(0, 1, false))
//		{
//			if (ImGui::MenuItem("Create Material"))
//			{
//				std::string path = m_CurrentDirectory.string() + "/newMaterial2.material";
//				Material::CreateDefaultMaterial(path);
//				CreateDirectoryItems();
//				Application::Get().GetAssetDatabase().CreateResource(path);
//				ImGui::CloseCurrentPopup();
//			}
//
//			ImGui::EndPopup();
//		}

		ImGui::End();
	}

	void ContentBrowserPanel::CreateDirectoryItems()
	{
		m_DirEntries.resize(0); 

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			m_DirEntries.push_back(directoryEntry);
		}
	}

	void ContentBrowserPanel::CreateDirectoryResources()
	{
		Timer timer("CreateDirectoryResources");
		m_Icons.resize(0);
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if(directoryEntry.path().extension() == ".png" || directoryEntry.path().extension() == ".jpg")
			{
				m_Icons.push_back(CreateTexture(directoryEntry.path(), Application::Get().GetAssetDatabase().GetUUID(directoryEntry.path()), m_RootDirectory.string()));
			}
		}
	}

	void DropCallback(GLFWwindow* window, int count, const char** paths)
	{
		for (int i = 0; i < count; i++)
		{
			std::filesystem::path path = paths[i];
#ifdef DUSK_WINDOWS
			std::string targetPath =  (*g_currentDir).string() + "\\" + path.filename().string();
#elif DUSK_LINUX
			std::string targetPath = (*g_currentDir).string() + "/" + path.filename().string();
#endif

			if(!std::filesystem::exists(targetPath))
			{
				std::filesystem::copy(paths[i], *g_currentDir);
				Application::Get().GetAssetDatabase().CreateResource(targetPath);
			}
			else
			{
				std::filesystem::remove(targetPath);
				std::filesystem::copy(paths[i], *g_currentDir);
			}			
		}
	}
}