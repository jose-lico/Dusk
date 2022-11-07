#pragma once

#include "Utils/Profiling/Timer.h"

extern DuskEngine::Application* DuskEngine::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	DuskEngine::Application* app;

	app = DuskEngine::CreateApplication(argc, argv);

	DuskEngine::Timer::ResetRoot();
	DuskEngine::Timer::Stop = true;

	if(!app->GetCliOptions().Help)
		app->Run();

	delete app;

	return 0;
}
