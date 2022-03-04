#include "pch.h"
#include "ContentBrowserPanel.h"

#include "DuskEngine.h"

#include <IconsForkAwesome.h>

namespace DuskEngine
{
	const std::filesystem::path g_RootDirectory = "res";

	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_CurrentDirectory = g_RootDirectory; 
		m_FolderIcon = Texture::Create("res/editor/icons/folder.png");
		m_UnknownIcon = Texture::Create("res/editor/icons/question-mark.png");
		CreateDirectoryItems();
		CreateDirectoryResources();
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin(ICON_FK_FOLDER_OPEN "  Asset Browser");
		CheckFocus();

		if (m_CurrentDirectory != std::filesystem::path(g_RootDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
				CreateDirectoryItems();
				CreateDirectoryResources();
			}
		}

		int buttons_count = m_DirEntries.size();
		ImGuiStyle& style = ImGui::GetStyle();

		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		int i = 0;
		int textureIndex = 0;

		bool shouldBreak = false;

		for(auto& directoryEntry : m_DirEntries)
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.stem().string();
			auto extension = directoryEntry.path().extension();
			if (extension == ".meta")
				continue;

			ImGui::PushID(filenameString.c_str());

			ImGui::BeginGroup();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,0 });

			if (directoryEntry.is_directory())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((void*)m_FolderIcon->GetRendererID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::PopStyleColor();

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					m_CurrentDirectory /= path.filename();
					CreateDirectoryItems();
					CreateDirectoryResources();
					shouldBreak = true;
				}
			}
			else if (extension == ".png" || extension == ".jpg")
			{
				if (ImGui::ImageButton((void*)m_Icons[textureIndex++]->GetRendererID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
					APP_LOG("This is an image");

				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = path.c_str();
					ImGui::SetDragDropPayload("TEXTURE", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}
			}
			else
			{
				ImGui::ImageButton((void*)m_UnknownIcon->GetRendererID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			}

			ImGui::PopStyleVar();

			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 64 + 40;

			ImGui::AlignTextToFramePadding();

			ImGui::Text(filenameString.c_str());

			ImGui::EndGroup();

			if (i++ + 1 < buttons_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine(0.0f, 20.0f);

			ImGui::PopID();

			if (shouldBreak) break; // Only break here to avoid breaking ImGui
		}

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
		ResourceManager::CreateUUIDs(); // Will be reworked later...
		m_Icons.resize(0);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			auto extension = directoryEntry.path().extension();
			if(extension == ".png" || extension == ".jpg")
			{
				m_Icons.push_back(Texture::Create(directoryEntry.path().string()));
			}
		}
	}
}