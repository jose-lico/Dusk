#include "DuskEngine.h"
#include "EntryPoint.h"

#include "EditorLayer.h"

namespace DuskEngine
{
	class DuskEditor : public Application
	{
	public:
		DuskEditor()
		{
			PushLayer(new EditorLayer());
		}

		~DuskEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new DuskEditor();
	}
}