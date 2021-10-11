#pragma once

#include <glm/glm.hpp>

namespace DuskEngine
{
	struct MeshRenderer
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		MeshRenderer() = default;
		MeshRenderer(const MeshRenderer&) = default;
		MeshRenderer(const glm::vec4& color)
			: Color(color) {}
	};
}