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

		glm::vec3 cameraPos;
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
					cameraPos = transform.Position;
				}
			}
		}

		{
			auto view = m_Registry.view<Transform, MeshRenderer>();
			for (auto entity : view)
			{
				auto& [transform, mesh] = view.get<Transform, MeshRenderer>(entity);

				// 1. Set "default" expected uniforms, whose data does not belong to the material - MVP, Lights
				// 2. Iterate thru remaining uniforms and set them based on material data - Textures.
				
				if(mesh.MaterialTeste)
				{
					mesh.MaterialTeste->SetUniforms();
				}
				else
				{
					mesh.SH->Bind();
					mesh.TX->Bind(0);
				}

				transform.Model = glm::translate(glm::mat4(1.0f), transform.Position)
					* glm::toMat4(glm::quat(transform.Rotation))
					* glm::scale(glm::mat4(1.0f), transform.Scale);

				mesh.SH->SetUniformMat4("e_Model", transform.Model);
				mesh.SH->SetUniformMat4("e_ViewProjection", VPM);
				mesh.SH->SetUniformVec3("e_ViewPosition", cameraPos);

				auto lights = m_Registry.view<Light>();
				
				for(auto light : lights)
				{
					auto& lightTransform = m_Registry.get<Transform>(light);
					glm::vec3 lightColor = m_Registry.get<Light>(light).Color;
					mesh.SH->SetUniformVec3("e_LightPosition", lightTransform.Position);
					mesh.SH->SetUniformVec3("e_LightDirection", lightTransform.Front);
					mesh.SH->SetUniformVec3("e_LightColor", lightColor);
				}

				Renderer::Submit(mesh.MS);
			}
		}
	}
}

