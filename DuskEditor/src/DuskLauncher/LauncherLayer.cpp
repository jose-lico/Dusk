#include "LauncherLayer.h"

#include "DuskEditor.h"

#include "Core/Application/Core.h"
#include "Core/Application/Window.h"
#include "Platform/OpenGL/OpenGLAPI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "yaml-cpp/yaml.h"

#include <fstream>
#include <filesystem>

#undef CreateWindow

namespace DuskEngine
{
	LauncherLayer::LauncherLayer(DuskEditor* editor)
		:m_Editor(editor)
	{
		m_Logger = new Logger(LOGGER);

		auto& app = Application::Get();

		WindowData data;
		const ApplicationSpecs& specs = app.GetSpecs();
		data.Title = specs.Name + " | " + specs.Platform + " | " + specs.Target + " | Launcher";
		Window& window = app.CreateWindow(data);
		OpenGLAPI::CreateContext(window.GetNativeHandle());
		app.SetImGuiGLContext();
		window.CenterWindow();

		auto& cache = app.GetOS().GetCacheDir();

		if(!std::filesystem::exists(cache))
		{
			std::filesystem::create_directory(cache);
			TRACE("Created cache directory");
		}

		m_ProjectListPath = cache / "projectList.yaml";

		if (std::filesystem::exists(m_ProjectListPath))
		{
			YAML::Node projectsData = YAML::LoadFile(m_ProjectListPath.string());
			YAML::Node projects = projectsData["Projects"];

			if (projects)
			{
				for (auto project : projects)
				{
					Project p{ project["Project"].as<std::string>(), project["Path"].as<std::string>() };
					m_Projects.emplace_back(p);
				}
			}

			TRACE("Fetched project list from AppData");
		}
		// Only usefull if I can append ...
		/*else
		{
			YAML::Emitter out;

			out << YAML::BeginMap;
			out << YAML::Key << "Projects" << YAML::Value << YAML::BeginSeq;

			std::ofstream fout(m_ProjectListPath);
			fout << out.c_str();

			TRACE("Created project list in AppData");
		}*/
	}

	LauncherLayer::~LauncherLayer()
	{
		delete m_Logger;
	}

	void LauncherLayer::OnUpdate()
	{
		if(m_LaunchEditor)
			m_Editor->LaunchEditor();
	}

	void LauncherLayer::OnImGuiRender()
	{
		m_Dockspace.BeginDockspace();

		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::Begin("Launcher");
		
		static char name[32] = "";
		ImGui::InputTextWithHint("##Project Name", "Project Name", name, IM_ARRAYSIZE(name));
		static char path[256] = "";
		ImGui::InputTextWithHint("##Project Path", "Project Path", path, IM_ARRAYSIZE(path));

		if(ImGui::Button("Import Project") && path[0] != 0 && name[0] != 0)
		{
			YAML::Emitter out;

			out << YAML::BeginMap;
			out << YAML::Key << "Projects" << YAML::Value << YAML::BeginSeq;

			// Dont know how to append correctly, rewrite whole file :/

			for (Project& project : m_Projects)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Project" << YAML::Value << project.Name;
				out << YAML::Key << "Path" << YAML::Value << project.Path;
				out << YAML::EndMap;
			}
			
			out << YAML::BeginMap;
			out << YAML::Key << "Project" << YAML::Value << name;
			out << YAML::Key << "Path" << YAML::Value << path;
			out << YAML::EndMap;

			std::ofstream fout(m_ProjectListPath);
			fout << out.c_str();
			m_LaunchEditor = true;
			m_Editor->ProjectPath = path;
		}

		ImGui::Separator();

		for(Project& project : m_Projects)
		{
			if(ImGui::Button(project.Name.c_str()))
			{
				m_LaunchEditor = true;
				m_Editor->ProjectPath = project.Path;
			}
			ImGui::SameLine();
			ImGui::Text("at path: %s", project.Path.c_str());
		}
		
		ImGui::End();
		m_Dockspace.EndDockspace();
	}
}
