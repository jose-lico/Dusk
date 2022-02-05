#pragma once
#include "PanelBase.h"

#include "Core/Renderer/Resources/Texture.h"

#include <imgui/imgui.h>

#include <filesystem>

namespace DuskEngine
{
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