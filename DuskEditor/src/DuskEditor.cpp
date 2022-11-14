#include "DuskEditor.h"
#include "EntryPoint.h"

#include "Utils/Profiling/Timer.h"
#include "DuskEditor/EditorLayer.h"
#include "DuskLauncher/LauncherLayer.h"

namespace DuskEngine
{
	DuskEditor::DuskEditor(const ApplicationSpecs& specs, const CliOptions& options)
		: Application(specs, options)
	{
		if (!m_Options.Help)
		{
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
		m_Layer = new EditorLayer();

		PushLayer(m_Layer);
	}

	Application* CreateApplication(int argc, char** argv)
	{
		Timer startup("Startup");

		ApplicationSpecs spec;
		spec.Name = "Dusk Editor";

		CliOptions options;
		options.ExeName = argv[0];

		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-h") == 0) {
				options.Help = true;
			}
			else if (strcmp(argv[i], "--help") == 0) {
				options.Help = true;
			}
			else if (strcmp(argv[i], "-v") == 0) {
				options.Verbose = true;
			}
			else if (strcmp(argv[i], "--verbose") == 0) {
				options.Verbose = true;
			}
			else if (strcmp(argv[i], "-l") == 0) {
				options.DumpLogs = true;
			}
			else if (strcmp(argv[i], "--logs") == 0) {
				options.DumpLogs = true;
			}
		}

		return new DuskEditor(spec, options);
	}
}