#pragma once

#include <glm/glm.hpp>

namespace DuskEngine
{
	struct Camera	
	{
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 viewProjectionMatrix;

		bool main = true;
	};
}