	#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "rttr/registration.h"
#include "visit_struct/visit_struct.hpp"

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


		glm::mat4 GetTransform() const
		{
			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), position)
				* rot
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};	
}

VISITABLE_STRUCT(DuskEngine::Transform, position, rotation, scale);

//RTTR_REGISTRATION
//{
//	rttr::registration::class_<DuskEngine::Transform>("Transform").
//		property("position", &DuskEngine::Transform::position).
//		property("rotation", &DuskEngine::Transform::rotation).
//		property("scale", &DuskEngine::Transform::scale);
//}