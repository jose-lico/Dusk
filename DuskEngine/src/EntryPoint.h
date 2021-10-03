#pragma once

extern DuskEngine::Application* app;

int main()
{
	//Initialize application
	//app->Init();

	// Application loop
	app->Run();

	// Shutdown application
	app->Shutdown();

	delete app;
	return 0;
}