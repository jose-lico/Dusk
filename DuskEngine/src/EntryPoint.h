#pragma once

extern DuskEngine::Application* DuskEngine::CreateApplication();

int main()
{
	auto app = DuskEngine::CreateApplication();
	
	// Application loop
	app->Run();

	delete app;
	return 0;
}