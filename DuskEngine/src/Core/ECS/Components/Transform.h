#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DuskEngine
{
	struct Transform
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::vec3 Front = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Right = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Up = { 0.0f, 0.0f, 0.0f };

		glm::mat4 Model = glm::mat4(1.0f);

		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& position)
			: Position(position) {}
	};
}