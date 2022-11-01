#pragma once

#include "Utils/Profiling/Timer.h"

extern DuskEngine::Application* DuskEngine::CreateApplication();

int main()
{
	DuskEngine::Application* app;

	{
		DuskEngine::Timer startup("Startup");
		app = DuskEngine::CreateApplication();
	}

	app->Run();

	delete app;

	return 0;
}
