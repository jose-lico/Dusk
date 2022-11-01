#pragma once

extern DuskEngine::Application* DuskEngine::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	DuskEngine::Application* app;

	app = DuskEngine::CreateApplication(argc, argv);

	if(!app->GetCmdOptions().Help)
		app->Run();

	delete app;

	return 0;
}
