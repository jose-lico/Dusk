#include "DuskEngine.h"
#include "EntryPoint.h"

#include "EditorLayer.h"

class DuskEditor : public DuskEngine::Application
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

DuskEngine::Application* DuskEngine::CreateApplication()
{
	return new DuskEditor();
}