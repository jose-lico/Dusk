#include "pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "Core/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

namespace DuskEngine
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<Transform>();
		entity.AddComponent<Meta>(name);
		return entity;
	}

	void Scene::OnUpdate()
	{
		{
			// TODO: for static objects in the future only calculate this once and not every frame

			// Should I/Do I need to even calculate Front, Right and Up vectors every frame? probably not

			auto view = m_Registry.view<Transform>();
			for (auto entity : view)
			{
				auto& transform = view.get<Transform>(entity);

				glm::vec3 front;

				front.x = cos(transform.Rotation.y) * cos(transform.Rotation.x);
				front.y = sin(transform.Rotation.x);
				front.z = sin(transform.Rotation.y) * cos(transform.Rotation.x);

				transform.Front = glm::normalize(front);
				transform.Right = glm::normalize(glm::cross(transform.Front, glm::vec3(0.0f, 1.0f, 0.0f)));
				transform.Up = glm::normalize(glm::cross(transform.Right, transform.Front));	
			}
		}

		glm::mat4 VPM;
		{
			auto view = m_Registry.view<Transform, Camera>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<Transform, Camera>(entity);

				if(camera.MainCamera)
				{
					camera.ViewMatrix= glm::lookAt(transform.Position, transform.Position + transform.Front, transform.Up);

					camera.ViewProjectionMatrix = camera.ProjectionMatrix * camera.ViewMatrix;
					VPM = camera.ViewProjectionMatrix;
				}
			}
		}

		{
			auto view = m_Registry.view<Transform, MeshRenderer>();
			for (auto entity : view)
			{
				auto& [transform, mesh] = view.get<Transform, MeshRenderer>(entity);

				mesh.TX->Bind(0);
				mesh.SH->Bind();

				transform.Model = glm::translate(glm::mat4(1.0f), transform.Position)
					* glm::toMat4(glm::quat(transform.Rotation))
					* glm::scale(glm::mat4(1.0f), transform.Scale);

				mesh.SH->SetUniformMat4("u_Model", transform.Model);
				mesh.SH->SetUniformMat4("u_ViewProjection", VPM);

				Renderer::Submit(mesh.MS);
			}
		}
	}
}

