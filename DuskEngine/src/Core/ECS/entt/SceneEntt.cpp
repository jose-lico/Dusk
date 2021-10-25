#include "pch.h"
#include "SceneEntt.h"

#include "Entity.h"
#include "Components.h"
#include "Core/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

namespace DuskEngine
{
	SceneEntt::SceneEntt(std::shared_ptr<DuskEngine::Camera>& camera)
		:m_Camera(camera)
	{
	}

	SceneEntt::~SceneEntt()
	{
	}

	Entity SceneEntt::CreateEntity()
	{
		Entity entity = { m_Registry.create(), this };
		return entity;
	}

	void SceneEntt::OnUpdate()
	{
		{
			// TODO: for static objects in the future only calculate this once and not every frame

			auto view = m_Registry.view<Transform>();
			for (auto entity : view)
			{
				auto transform = view.get<Transform>(entity);

				glm::vec3 front;

				front.x = cos(glm::radians(transform.Rotation.y)) * cos(glm::radians(transform.Rotation.x));
				front.y = sin(glm::radians(transform.Rotation.x));
				front.z = sin(glm::radians(transform.Rotation.y)) * cos(glm::radians(transform.Rotation.y));

				transform.Front = glm::normalize(front);

				transform.Right = glm::normalize(glm::cross(transform.Front, glm::vec3(0.0f, 1.0f, 0.0f)));

				transform.Up = glm::normalize(glm::cross(transform.Right, transform.Front));

			}
		}

		{
			// TODO: get cameras and get vp matrix here. For now scene has a camera obj

			//auto view = m_Registry.view<Camera>();
		}

		{
			auto view = m_Registry.view<Transform, MeshRenderer>();
			for (auto entity : view)
			{
				auto [transform, mesh] = view.get<Transform, MeshRenderer>(entity);

				mesh.TX->Bind(0);
				mesh.SH->Bind();

				glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.Position)
					* glm::toMat4(glm::quat(transform.Rotation))
					* glm::scale(glm::mat4(1.0f), transform.Scale);

				mesh.SH->SetUniformMat4("u_Model", model);
				mesh.SH->SetUniformMat4("u_ViewProjection", m_Camera->GetViewProjectionMatrix());

				Renderer::Submit(mesh.VA);
			}
		}
	}
}

