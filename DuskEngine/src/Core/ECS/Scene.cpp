#include "pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "EditorCamera.h"

#include "Core/Application/Application.h"
#include "Core/Application/Window.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/Shader.h"
#include "Core/Assets/Assets/Material.h"
#include "Core/Assets/AssetHandler.h"
#include "Core/Scripting/ScriptingEngine.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Core/Assets/Assets/Mesh.h"

const unsigned int MAX_LIGHTS = 8;

namespace DuskEngine
{
	// maybe reserve ahead of time # of entities present in the base scene
	Scene::Scene(const std::string& name)
		:m_Name(name)
	{
		std::string message = "Creating new scene " + m_Name;
		LOG(message.c_str());
		//m_GridShader = MakeRef<Shader>("res/editor/shaders/grid.glsl");

		m_ScriptingEngine = new ScriptingEngine();
		m_AssetHandler = new AssetHandler("SceneHandler");
	}

	Scene::~Scene()
	{
		std::string message = "Destroying scene " + m_Name;
		LOG(message.c_str());

		delete(m_AssetHandler);
		delete(m_ScriptingEngine);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<Transform>();
		entity.AddComponent<Meta>(name);
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

		glm::quat quat = glm::quat(transform.rotation);

		front = quat * glm::vec3(0.0f, 0.0f, 1.0f);

		transform.front = glm::normalize(front);
		transform.right = glm::normalize(glm::cross(transform.front, glm::vec3(0.0f, 1.0f, 0.0f)));
		transform.up = glm::normalize(glm::cross(transform.right, transform.front));

		camera.camera.viewMatrix = glm::lookAt(transform.position, transform.position + transform.front, transform.up);
		camera.camera.viewProjectionMatrix = camera.camera.projectionMatrix * camera.camera.viewMatrix;

		glm::vec3 cameraPos = camera.transform.position;
		glm::mat4 VPM = camera.camera.viewProjectionMatrix;

		auto& renderer = Application::Get().GetRenderer();

		renderer.BeginScene();

		OpenGLAPI::SetClearColor({ 0.192f, 0.301f, 0.474f, 1.0f });
		OpenGLAPI::Clear();
		{
			// Most of this should maybe be moved to the renderer
			auto view = m_Registry.view<Transform, MeshRenderer, Meta>();
			for (auto entity : view)
			{
				auto [transform, mesh, meta] = view.get<Transform, MeshRenderer, Meta>(entity);

				if (!meta.enabled) continue;

				auto& material = m_AssetHandler->GetAsset<Material>(mesh.materialHandle);

				material.UploadUniforms(*m_AssetHandler);

				transform.model = glm::translate(glm::mat4(1.0f), transform.position)
					* glm::toMat4(glm::quat(transform.rotation))
					* glm::scale(glm::mat4(1.0f), transform.scale);

				Shader& shader = m_AssetHandler->GetAsset<Shader>(material.m_ShaderHandle);

				// Expected Uniforms
				OpenGLAPI::SetUniformMat4(shader, "e_Model", transform.model);
				OpenGLAPI::SetUniformMat4(shader, "e_ViewProjection", VPM);
				OpenGLAPI::SetUniformVec3(shader, "e_ViewPosition", cameraPos);

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
							OpenGLAPI::SetUniformInt(shader, "e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Enabled", 1);
							OpenGLAPI::SetUniformVec3(shader, "e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Color", lightColor);
							OpenGLAPI::SetUniformVec3(shader, "e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Direction", t.front);
							break;
						case LightType::Point:
							OpenGLAPI::SetUniformInt(shader, "e_PointLights[" + std::to_string(pointLightIndex) + "].Enabled", 1);
							OpenGLAPI::SetUniformVec3(shader, "e_PointLights[" + std::to_string(pointLightIndex) + "].Color", lightColor);
							OpenGLAPI::SetUniformVec3(shader, "e_PointLights[" + std::to_string(pointLightIndex++) + "].Position", t.position);
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
							OpenGLAPI::SetUniformInt(shader, "e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Enabled", 0);
							break;
						case LightType::Point:
							OpenGLAPI::SetUniformInt(shader, "e_PointLights[" + std::to_string(pointLightIndex++) + "].Enabled", 0);
							break;
						case LightType::Spot:
							break;
						default:
							break;
						}
					}
				}

				OpenGLAPI::SetUniformInt(shader, "e_DirectionalLightsCount", dirLightIndex);
				OpenGLAPI::SetUniformInt(shader, "e_PointLightsCount", pointLightIndex);

				renderer.Submit(m_AssetHandler->GetAsset(mesh.meshHandle));
			}

			//glm::mat4 viewMatrix = camera.camera.viewMatrix;
			//glm::mat4 projectionMatrix = camera.camera.projectionMatrix;

			/*m_GridShader->Bind();
			m_GridShader->SetUniformMat4("e_ViewProjection", VPM);
			m_GridShader->SetUniformMat4("e_Projection", projectionMatrix);
			m_GridShader->SetUniformMat4("e_View", viewMatrix);

			OpenGLAPI::UnbindVAO();
			OpenGLAPI::DrawArrays(0, 6);*/

			renderer.EndScene();
		}
	}

	void Scene::OnAwakeRuntime()
	{
		auto view = m_Registry.view<Script>();
		for (auto entity : view)
		{
			auto& script = view.get<Script>(entity);
			m_ScriptingEngine->LoadScript(m_AssetHandler->LuaScriptPool(script.luaScriptHandle));
			m_ScriptingEngine->OnAwake(m_AssetHandler->LuaScriptPool(script.luaScriptHandle));
		}
	}

	void Scene::OnUpdateRuntime(bool running, bool paused)
	{
		if(running && !paused)
		{
			auto view = m_Registry.view<Script>();
			for (auto entity : view)
			{
				auto& script = view.get<Script>(entity);
				m_ScriptingEngine->OnUpdate(m_AssetHandler->LuaScriptPool(script.luaScriptHandle));
			}
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

				glm::quat quat = glm::quat(transform.rotation);

				front = quat * glm::vec3(0.0f, 0.0f, 1.0f);

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

		auto& renderer = Application::Get().GetRenderer();

		renderer.BeginScene();

		OpenGLAPI::SetClearColor({ 0.192f, 0.301f, 0.474f, 1.0f });
		OpenGLAPI::Clear();
		{
			// Most of this should maybe be moved to the renderer
			auto view = m_Registry.view<Transform, MeshRenderer, Meta>();
			for (auto entity : view)
			{
				auto [transform, mesh, meta] = view.get<Transform, MeshRenderer, Meta>(entity);

				if (!meta.enabled) continue;

				auto& material = m_AssetHandler->GetAsset<Material>(mesh.materialHandle);

				material.UploadUniforms(*m_AssetHandler);

				transform.model = glm::translate(glm::mat4(1.0f), transform.position)
					* glm::toMat4(glm::quat(transform.rotation))
					* glm::scale(glm::mat4(1.0f), transform.scale);

				Shader& shader = m_AssetHandler->GetAsset<Shader>(material.m_ShaderHandle);

				// Expected Uniforms
				OpenGLAPI::SetUniformMat4(shader, "e_Model", transform.model);
				OpenGLAPI::SetUniformMat4(shader, "e_ViewProjection", VPM);
				OpenGLAPI::SetUniformVec3(shader, "e_ViewPosition", cameraPos);

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
								OpenGLAPI::SetUniformInt(shader, "e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Enabled", 1);
								OpenGLAPI::SetUniformVec3(shader, "e_DirectionalLights[" + std::to_string(dirLightIndex) + "].Color", lightColor);
								OpenGLAPI::SetUniformVec3(shader, "e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Direction", t.front);
								break;
							case LightType::Point:
								OpenGLAPI::SetUniformInt(shader, "e_PointLights[" + std::to_string(pointLightIndex) + "].Enabled", 1);
								OpenGLAPI::SetUniformVec3(shader, "e_PointLights[" + std::to_string(pointLightIndex) + "].Color", lightColor);
								OpenGLAPI::SetUniformVec3(shader, "e_PointLights[" + std::to_string(pointLightIndex++) + "].Direction", t.position);
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
								OpenGLAPI::SetUniformInt(shader, "e_DirectionalLights[" + std::to_string(dirLightIndex++) + "].Enabled", 0);
								break;
							case LightType::Point:
								OpenGLAPI::SetUniformInt(shader, "e_PointLights[" + std::to_string(pointLightIndex++) + "].Enabled", 0);
								break;
							case LightType::Spot:
								break;
						}
					}
				}

				OpenGLAPI::SetUniformInt(shader, "e_DirectionalLightsCount", dirLightIndex);
				OpenGLAPI::SetUniformInt(shader, "e_PointLightsCount", pointLightIndex);

				renderer.Submit(m_AssetHandler->GetAsset(mesh.meshHandle));
			}

			renderer.EndScene();
		}
	}

	void Scene::OnShutdownRuntime()
	{
		auto view = m_Registry.view<Script>();
		for (auto entity : view)
		{
			auto& script = view.get<Script>(entity);
			m_ScriptingEngine->OnShutdown(m_AssetHandler->LuaScriptPool(script.luaScriptHandle));
		}
	}
}
