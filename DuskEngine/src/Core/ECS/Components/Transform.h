#pragma once

#include <glm/glm.hpp>

namespace DuskEngine
{
	struct TransformC
	{
		glm::mat4 Transform{ 1.0f };

		TransformC() = default;
		TransformC(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
	};
}