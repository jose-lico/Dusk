#pragma once
#include "PanelBase.h"

#include "Platform/OpenGL/Texture.h"

#include <filesystem>
#include <vector>
#include <unordered_map>

namespace uuids
{
	class uuid;
}

namespace DuskEngine
{
	struct Texture;
	class AssetDatabaseEditor;
	class InspectorPanel;

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel(const std::string& projectPath, AssetDatabaseEditor& database, InspectorPanel& inspector);
		~ContentBrowserPanel();

		virtual void OnImGuiRender() override;
		void CreateDirectoryItems();
		void CreateDirectoryResources();
	private:
		Texture m_FolderIcon, m_UnknownIcon;
		AssetDatabaseEditor* m_Database;
		InspectorPanel* m_InspectorPanel;

		std::filesystem::path m_CurrentDirectory;
		std::filesystem::path m_RootDirectory;
		
		std::vector<std::filesystem::directory_entry> m_DirEntries;
		std::vector<Texture> m_IconsInDirectory;
		std::unordered_map<uuids::uuid ,Texture> m_IconsInProject;

		unsigned int m_EditingName = -1;
		std::filesystem::path m_EditingPath;
	};
}