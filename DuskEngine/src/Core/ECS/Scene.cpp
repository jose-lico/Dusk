#include "pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderCommand.h"

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

	Entity* Scene::GetMainCamera()
	{
		auto view = m_Registry.view<Camera>();

		for (auto entity : view)
		{
			auto& camera = view.get<Camera>(entity);
			if (camera.main)
				return new Entity(entity, this);
		}

		return nullptr;
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

				front.x = cos(transform.rotation.y) * cos(transform.rotation.x);
				front.y = sin(transform.rotation.x);
				front.z = sin(transform.rotation.y) * cos(transform.rotation.x);

				transform.front = glm::normalize(front);
				transform.right = glm::normalize(glm::cross(transform.front, glm::vec3(0.0f, 1.0f, 0.0f)));
				transform.up = glm::normalize(glm::cross(transform.right, transform.front));	
			}
		}

		glm::vec3 cameraPos;
		glm::mat4 VPM;
		{
			auto view = m_Registry.view<Transform, Camera>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<Transform, Camera>(entity);

				if(camera.main)
				{
					camera.viewMatrix= glm::lookAt(transform.position, transform.position + transform.front, transform.up);

					camera.viewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix;
					VPM = camera.viewProjectionMatrix;
					cameraPos = transform.position;
				}
			}
		}

		Renderer::BeginScene();

		RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1 });
		RenderCommand::Clear();
		{
			// Most of this should maybe be moved to the renderer
			auto view = m_Registry.view<Transform, MeshRenderer>();
			for (auto entity : view)
			{
				auto& [transform, mesh] = view.get<Transform, MeshRenderer>(entity);

				mesh.material->UploadUniforms();

				transform.model = glm::translate(glm::mat4(1.0f), transform.position)
					* glm::toMat4(glm::quat(transform.rotation))
					* glm::scale(glm::mat4(1.0f), transform.scale);

				mesh.material->m_Shader->SetUniformMat4("e_Model", transform.model);
				mesh.material->m_Shader->SetUniformMat4("e_ViewProjection", VPM);
				mesh.material->m_Shader->SetUniformVec3("e_ViewPosition", cameraPos);

				auto lights = m_Registry.view<Light>();

				int dirLightIndex = 0;
				int pointLightIndex = 0;

				// this is very dodgy, redo later, probably with specific structs for each light

				for (size_t i = 0; i < 8; i++) // max lights
				{
					mesh.material->m_Shader->SetUniformVec3("e_DirectionalLights[" + std::to_string(i) + "].Color", glm::vec3(0.0));
					mesh.material->m_Shader->SetUniformVec3("e_PointLights[" + std::to_string(i) + "].Color", glm::vec3(0.0));
				}

				for (auto light : lights)
				{
					auto& [l, t] = m_Registry.get<Light, Transform>(light);
					glm::vec3 lightColor = l.color;
					switch(l.type)
					{
						case LightType::Directional:
							mesh.material->m_Shader->SetUniformVec3("e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Color", lightColor);
							mesh.material->m_Shader->SetUniformVec3("e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Direction", t.front);
							break;
						case LightType::Point:
							mesh.material->m_Shader->SetUniformVec3("e_PointLights[" + std::to_string(pointLightIndex) + "].Color", lightColor);
							mesh.material->m_Shader->SetUniformVec3("e_PointLights[" + std::to_string(pointLightIndex++) + "].Position", t.position);
							break;
						case LightType::Spot:
							break;
					}
				}

				mesh.material->m_Shader->SetUniformInt("e_DirectionalLightsCount", dirLightIndex);
				mesh.material->m_Shader->SetUniformInt("e_PointLightsCount", pointLightIndex);

				Renderer::Submit(mesh.mesh);
			}

			Renderer::EndScene();
		}
	}
}

