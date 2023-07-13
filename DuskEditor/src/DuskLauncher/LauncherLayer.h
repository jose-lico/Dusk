#pragma once
#include "Core/Application/Layer.h"

#include "Dockspace.h"

#include <vector>
#include <filesystem>

namespace DuskEngine
{
	class Logger;
	class DuskEditor;

	struct Project
	{
		std::string Name;
		std::string Path;
	};

	class LauncherLayer : public Layer
	{
	public:
		LauncherLayer(DuskEditor* editor);
		~LauncherLayer();

		void OnUpdate() override;
		void OnImGuiRender() override;
	private:
		Dockspace m_Dockspace;
		std::vector<Project> m_Projects;
		std::filesystem::path m_ProjectListPath;

		DuskEditor* m_Editor;
		Logger* m_Logger;
		
		bool m_LaunchEditor = false;
	};
}