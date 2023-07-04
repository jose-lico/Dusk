#include "DuskEditor.h"
#include "EntryPoint.h"

#include "Utils/Profiling/Timer.h"
#include "DuskEditor/EditorLayer.h"
#include "DuskLauncher/LauncherLayer.h"
#include "Core/Application/Window.h"

namespace DuskEngine
{
	DuskEditor::DuskEditor(const ApplicationSpecs& specs, const CliOptions& options)
		: Application(specs, options)
	{
		if (!m_Options.Help)
		{
			if(m_Options.Path)
				m_Layer = new EditorLayer("teste");
			else	
				m_Layer = new LauncherLayer(this);
			PushLayer(m_Layer);
		}
	}

	DuskEditor::~DuskEditor()
	{
	}

	void DuskEditor::LaunchEditor()
	{
		PopLayer(m_Layer);
		delete m_Layer;

		DestroyImGuiGLContext();
		delete m_Window;

		m_ProjectPath = ProjectPath;
		m_Layer = new EditorLayer(ProjectPath);
		PushLayer(m_Layer);
	}

	Application* CreateApplication(int argc, char** argv)
	{
		ScopeTimer startup("Startup");

		ApplicationSpecs spec;
		spec.Name = "Dusk Editor";

		CliOptions options;
		options.ExeName = argv[0];

		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
				options.Help = true;
			}
			else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
				options.Verbose = true;
			}
			else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--logs") == 0) {
				options.DumpLogs = true;
			}
			else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--path") == 0) {
				options.Path = true;
				if(argc < i + 1)
					options.PathName = argv[i + 1];
			}
		}

		return new DuskEditor(spec, options);
	}
}