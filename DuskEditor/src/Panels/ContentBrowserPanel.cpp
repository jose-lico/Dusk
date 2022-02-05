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
		ImGui::Begin(ICON_FK_FOLDER_OPEN " Asset Browser");
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
			//auto relativePath = std::filesystem::relative(path, g_RootDirectory);
			std::string filenameString = path.stem().string();
			auto extension = directoryEntry.path().extension();
			if (extension == ".meta")
				continue;

			ImGui::PushID(filenameString.c_str());

			ImGui::BeginGroup();
			//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,0 });
			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 100,0 });
			ImGui::Spacing();

			Ref<Texture> icon;

			if (directoryEntry.is_directory())
			{
				ImGui::ImageButton((void*)m_FolderIcon->GetRendererID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

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
			}
			else
			{
				ImGui::ImageButton((void*)m_UnknownIcon->GetRendererID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			}

			//ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			//ImGui::PopStyleColor();



			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 64 + 40;

			/*if(filenameString.length() > 10)
			{
				filenameString = filenameString.substr(0, 8);
				filenameString += "...";
			}*/

			ImGui::AlignTextToFramePadding();

			ImGui::Text(filenameString.c_str());

			// for text edit when late double click
			//ImGui::PushItemWidth(64);
			//ImGui::PopItemWidth();

			ImGui::EndGroup();

			if (i++ + 1 < buttons_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine(0.0f, 40.0f);

			ImGui::PopID();

			if (shouldBreak) break; // only break here because of imgui stuff
		}

		/*if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("New camera"))
			{
			}

			ImGui::EndPopup();
		}*/

		ImGui::End();
	}

	void ContentBrowserPanel::CreateDirectoryItems()
	{
		m_DirEntries.resize(0); 
		// could resize with std::distance

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			m_DirEntries.push_back(directoryEntry);
		}
	}

	void ContentBrowserPanel::CreateDirectoryResources()
	{
		m_Icons.resize(0);
		// could resize with std::distance

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