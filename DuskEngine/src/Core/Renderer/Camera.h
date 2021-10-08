#pragma once
#include "Core/Macros/DUSK_API.h"

#include <glm/glm.hpp>

namespace DuskEngine
{
	class DUSK_API Camera
	{
	public:
		Camera(const glm::mat4& projection, glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f));
		~Camera();

		const glm::vec3& GetPosition();
		const glm::vec3& GetRotation();
		const glm::mat4& GetProjectionMatrix();
		const glm::mat4& GetViewMatrix();
		const glm::mat4& GetViewProjectionMatrix();

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
	private:
		class CameraImpl;
		CameraImpl* m_Impl;
	};
}