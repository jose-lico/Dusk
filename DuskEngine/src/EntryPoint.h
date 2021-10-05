#pragma once

extern DuskEngine::Application* app;

int main()
{
	// Application loop
	app->Run();

	delete app;
	return 0;
}