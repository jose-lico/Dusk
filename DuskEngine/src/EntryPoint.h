#pragma once

extern DuskEngine::Application* DuskEngine::CreateApplication();

int main()
{
	auto app = DuskEngine::CreateApplication();

	app->Run();

	delete app;
	return 0;
}