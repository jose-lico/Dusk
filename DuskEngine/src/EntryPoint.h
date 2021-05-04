#pragma once

extern DuskEngine::Application* app;

int main()
{
	app->Run();
	delete app;
	return 0;
}