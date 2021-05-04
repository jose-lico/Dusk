#pragma once

#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include <iostream>

extern DuskEngine::Application* app;

int main()
{
	glm::vec2 vector2(5, 6);
	std::cout << glm::to_string(vector2) << std::endl;
	app->Run();
	delete app;
	return 0;
}