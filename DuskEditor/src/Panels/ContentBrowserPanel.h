#pragma once
#include "PanelBase.h"

#include "Utils/Memory/Memory.h"
#include "Platform/OpenGL/Texture.h"

#include "imgui/imgui.h"

#include <filesystem>
#include <vector>

namespace DuskEngine
{
	struct Texture;

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
		Texture m_FolderIcon, m_UnknownIcon;

		unsigned int m_EditingName = -1;
		std::filesystem::path m_EditingPath;
	};
}