#include "pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "EditorCamera.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Resources/Resources/Shader.h"
#include "Core/Scripting/ScriptingEngine.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"

const unsigned int MAX_LIGHTS = 8;

namespace DuskEngine
{
	// maybe reserve ahead of time # of entities present in the base scene
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

	Entity Scene::CreateEmptyEntity()
	{
		Entity entity = { m_Registry.create(), this };
		return entity;
	}

	

	void Scene::DestroyEntity(entt::entity handle)
	{
		m_Registry.destroy(handle);
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity.m_EntityHandle);
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

	Entity* Scene::FindEntity(const std::string& name)
	{
		auto view = m_Registry.view<Meta>();

		for (auto entity : view)
		{
			auto& ent = view.get<Meta>(entity);
			if (ent.name == name)
				return new Entity(entity, this);
		}

		return nullptr;
	}

	void Scene::OnUpdateEditor(EditorCamera& camera)
	{
		auto& transform = camera.transform;

		glm::vec3 front;

		front.x = cos(transform.rotation.y) * cos(transform.rotation.x);
		front.y = sin(transform.rotation.x);
		front.z = sin(transform.rotation.y) * cos(transform.rotation.x);

		transform.front = glm::normalize(front);
		transform.right = glm::normalize(glm::cross(transform.front, glm::vec3(0.0f, 1.0f, 0.0f)));
		transform.up = glm::normalize(glm::cross(transform.right, transform.front));

		camera.camera.viewMatrix = glm::lookAt(transform.position, transform.position + transform.front, transform.up);
		camera.camera.viewProjectionMatrix = camera.camera.projectionMatrix * camera.camera.viewMatrix;

		glm::vec3 cameraPos = camera.transform.position;
		glm::mat4 VPM = camera.camera.viewProjectionMatrix;

		Renderer::BeginScene();

		RenderCommand::SetClearColor({ 0.192f, 0.301f, 0.474f, 1.0f });
		RenderCommand::Clear();
		{
			// Most of this should maybe be moved to the renderer
			auto view = m_Registry.view<Transform, MeshRenderer, Meta>();
			for (auto entity : view)
			{
				auto [transform, mesh, meta] = view.get<Transform, MeshRenderer, Meta>(entity);

				if (!meta.enabled) continue;

				mesh.material->UploadUniforms();

				transform.model = glm::translate(glm::mat4(1.0f), transform.position)
					* glm::toMat4(glm::quat(transform.rotation))
					* glm::scale(glm::mat4(1.0f), transform.scale);

				// Expected Uniforms
				mesh.material->m_Shader->SetUniformMat4("e_Model", transform.model);
				mesh.material->m_Shader->SetUniformMat4("e_ViewProjection", VPM);
				mesh.material->m_Shader->SetUniformVec3("e_ViewPosition", cameraPos);

				auto lights = m_Registry.view<Light, Meta>();

				int dirLightIndex = 0;
				int pointLightIndex = 0;

				for (auto light : lights)
				{
					auto [l, t, m] = m_Registry.get<Light, Transform, Meta>(light);

					if (m.enabled)
					{
						glm::vec3 lightColor = l.color;
						switch (l.type)
						{
						case LightType::Directional:
							mesh.material->m_Shader->SetUniformInt(("e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Enabled"), 1);
							mesh.material->m_Shader->SetUniformVec3("e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Color", lightColor);
							mesh.material->m_Shader->SetUniformVec3("e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Direction", t.front);
							break;
						case LightType::Point:
							mesh.material->m_Shader->SetUniformInt(("e_PointLights[" + std::to_string(pointLightIndex) + "].Enabled"), 1);
							mesh.material->m_Shader->SetUniformVec3("e_PointLights[" + std::to_string(pointLightIndex) + "].Color", lightColor);
							mesh.material->m_Shader->SetUniformVec3("e_PointLights[" + std::to_string(pointLightIndex++) + "].Position", t.position);
							break;
						case LightType::Spot:
							break;
						}
						continue;
					}
					else
					{
						switch (l.type)
						{
						case LightType::Directional:
							mesh.material->m_Shader->SetUniformInt(("e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Enabled"), 0);
							break;
						case LightType::Point:
							mesh.material->m_Shader->SetUniformInt(("e_PointLights[" + std::to_string(pointLightIndex++) + "].Enabled"), 0);
							break;
						case LightType::Spot:
							break;
						default:
							break;
						}
					}
				}

				mesh.material->m_Shader->SetUniformInt("e_DirectionalLightsCount", dirLightIndex);
				mesh.material->m_Shader->SetUniformInt("e_PointLightsCount", pointLightIndex);

				Renderer::Submit(mesh.mesh);
			}

			Renderer::EndScene();
		}
	}

	void Scene::OnAwakeRuntime()
	{
		auto view = m_Registry.view<Script>();
		for (auto entity : view)
		{
			auto& script = view.get<Script>(entity);
			ScriptingEngine::OnAwake(script.scripts[0]);
		}
	}

	void Scene::OnUpdateRuntime(bool running)
	{
		/*if(playing)
		{
			auto view = m_Registry.view<Script>();
			for (auto entity : view)
			{
				auto& script = view.get<Script>(entity);
				script.script->OnUpdate();
			}
		}*/

		if(running)
		{
			//LOG("This is the scene playing")
		}

		{
			// TODO: for static objects in the future only calculate this once and not every frame

			// Should I/Do I need to even calculate Front, Right and Up vectors every frame? probably not
			auto view = m_Registry.view<Transform, Meta>();
			for (auto entity : view)
			{
				auto [transform,meta] = view.get<Transform, Meta>(entity);

				if (!meta.enabled) continue;

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
			auto view = m_Registry.view<Transform, Camera, Meta>();
			for (auto entity : view)
			{
				auto [transform, camera, meta] = view.get<Transform, Camera, Meta>(entity);

				if (!meta.enabled) continue;

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

		RenderCommand::SetClearColor({ 0.192f, 0.301f, 0.474f, 1.0f });
		RenderCommand::Clear();
		{
			// Most of this should maybe be moved to the renderer
			auto view = m_Registry.view<Transform, MeshRenderer, Meta>();
			for (auto entity : view)
			{
				auto [transform, mesh, meta] = view.get<Transform, MeshRenderer, Meta>(entity);

				if (!meta.enabled) continue;

				mesh.material->UploadUniforms();

				transform.model = glm::translate(glm::mat4(1.0f), transform.position)
					* glm::toMat4(glm::quat(transform.rotation))
					* glm::scale(glm::mat4(1.0f), transform.scale);

				// Expected Uniforms
				mesh.material->m_Shader->SetUniformMat4("e_Model", transform.model);
				mesh.material->m_Shader->SetUniformMat4("e_ViewProjection", VPM);
				mesh.material->m_Shader->SetUniformVec3("e_ViewPosition", cameraPos);

				auto lights = m_Registry.view<Light, Meta>();

				int dirLightIndex = 0;
				int pointLightIndex = 0;

				for (auto light : lights)
				{
					auto [l, t, m] = m_Registry.get<Light, Transform, Meta>(light);

					if (m.enabled) 
					{						
						glm::vec3 lightColor = l.color;
						switch (l.type)
						{
							case LightType::Directional:
								mesh.material->m_Shader->SetUniformInt(("e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Enabled"), 1);
								mesh.material->m_Shader->SetUniformVec3("e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Color", lightColor);
								mesh.material->m_Shader->SetUniformVec3("e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Direction", t.front);
								break;
							case LightType::Point:
								mesh.material->m_Shader->SetUniformInt(("e_PointLights[" + std::to_string(pointLightIndex) + "].Enabled"), 1);
								mesh.material->m_Shader->SetUniformVec3("e_PointLights[" + std::to_string(pointLightIndex) + "].Color", lightColor);
								mesh.material->m_Shader->SetUniformVec3("e_PointLights[" + std::to_string(pointLightIndex++) + "].Position", t.position);
								break;
							case LightType::Spot:
								break;
						}
						continue;
					}
					else 
					{
						switch (l.type)
						{
							case LightType::Directional:
								mesh.material->m_Shader->SetUniformInt(("e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Enabled"), 0);
							case LightType::Point:
								mesh.material->m_Shader->SetUniformInt(("e_PointLights[" + std::to_string(pointLightIndex++) + "].Enabled"), 0);
							case LightType::Spot:
								break;
						}
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

