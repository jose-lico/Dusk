#pragma once
#include "PanelBase.h"

#include "Utils/Memory/Memory.h"

#include "imgui/imgui.h"

#include <filesystem>
#include <vector>

namespace DuskEngine
{
	class Texture;

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		virtual void OnImGuiRender() override;
	private:
		void CreateDirectoryItems();
		void CreateDirectoryResources();
		std::filesystem::path m_CurrentDirectory;
		
		std::vector<Ref<Texture>> m_Icons;
		std::vector<std::filesystem::directory_entry> m_DirEntries;
		Ref<Texture> m_FolderIcon;
		Ref<Texture> m_UnknownIcon;
	};
}