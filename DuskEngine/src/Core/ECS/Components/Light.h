#pragma once

#include <glm/glm.hpp>

namespace DuskEngine
{
	enum class LightType { Directional, Point, Spot};

	struct Light
	{
		LightType type;
		glm::vec3 color;

		Light() = default;
	};
}