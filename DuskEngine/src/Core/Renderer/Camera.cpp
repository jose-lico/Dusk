#include "pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace DuskEngine
{
	class Camera::CameraImpl
	{
	public:
		CameraImpl(const glm::mat4& projection, glm::vec3 position, glm::vec3 rotation)
			:m_ProjectionMatrix(projection), m_ViewMatrix(1.0f), m_Position(position), m_Rotation(rotation) 
		{
			RecalculateViewMatrix();
		}

		const glm::vec3& GetPosition() { return m_Position; }
		const glm::vec3& GetRotation() { return m_Rotation; }
		const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
	private:
		void RecalculateViewMatrix();
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
	};

	void Camera::CameraImpl::RecalculateViewMatrix()
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

	Camera::Camera(const glm::mat4& projection, glm::vec3 position, glm::vec3 rotation)
		:m_Impl(new CameraImpl(projection, position, rotation))
	{
	}

	Camera::~Camera()
	{
		delete m_Impl;
	}

	const glm::vec3& Camera::GetPosition()
	{
		return m_Impl->GetPosition();
	}

	const glm::vec3& Camera::GetRotation()
	{
		return m_Impl->GetRotation();
	}

	const glm::mat4& Camera::GetProjectionMatrix()
	{
		return m_Impl->GetProjectionMatrix();
	}

	const glm::mat4& Camera::GetViewMatrix()
	{
		return m_Impl->GetViewMatrix();
	}

	const glm::mat4& Camera::GetViewProjectionMatrix()
	{
		return m_Impl->GetViewProjectionMatrix();
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Impl->SetPosition(position);
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{
		m_Impl->SetRotation(rotation);
	}
}