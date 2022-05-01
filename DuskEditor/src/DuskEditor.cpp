#include "Core/Application/Application.h"
#include "EntryPoint.h"

#include "EditorLayer.h"

namespace DuskEngine
{
	class DuskEditor : public Application
	{
	public:
		DuskEditor(const ApplicationSpecs& specs)
			: Application(specs)
		{
			PushLayer(new EditorLayer());
		}

		~DuskEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		// doesnt do anything right now
		ApplicationSpecs spec;
		spec.Name = "Dusk Editor";
		return new DuskEditor(spec);
	}
}