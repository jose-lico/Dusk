#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DuskEngine
{
	struct Transform
	{
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		glm::vec3 front = { 0.0f, 0.0f, 0.0f };
		glm::vec3 right = { 0.0f, 0.0f, 0.0f };
		glm::vec3 up = { 0.0f, 0.0f, 0.0f };

		glm::mat4 model = glm::mat4(1.0f);

		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& position)
			: position(position) {}
	};
}