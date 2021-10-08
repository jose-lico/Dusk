#include "DuskEngine.h"
#include "EntryPoint.h"

#include "ExampleLayer.h"

class Sandbox : public DuskEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

DuskEngine::Application* DuskEngine::CreateApplication()
{
	return new Sandbox();
}