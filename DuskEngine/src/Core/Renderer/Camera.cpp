#include "pch.h"
#include "Camera.h"

#include "gtc/matrix_transform.hpp"

namespace DuskEngine
{
	Camera::Camera(const glm::mat4& projection, glm::vec3 position, glm::vec3 rotation)
		:m_ProjectionMatrix(projection), m_ViewMatrix(1.0f), m_Position(position), m_Rotation(rotation)
	{
		RecalculateViewMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
		front.y = sin(glm::radians(m_Rotation.x));
		front.z = sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
		front = glm::normalize(front);

		glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::vec3 up = glm::normalize(glm::cross(right, front));

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + front, up);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}