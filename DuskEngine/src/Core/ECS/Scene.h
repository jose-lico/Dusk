#pragma once

#include "Core/Application/Core.h"
#include "Platform/OpenGL/Shader.h"

#include "entt/entity/registry.hpp"

namespace DuskEngine
{
	struct Camera;
	struct Transform;
	class Entity;
	class ScriptingEngine;
	class AssetHandler;

	class Scene
	{
	public:
		Scene(const std::string& name, const std::string& projectPath);
		~Scene();

		Entity CreateEntity(const std::string& name);
		
		void DestroyEntity(entt::entity handle);
		void DestroyEntity(Entity entity);
		
		template<typename T>
		Entity* FindEntityOfType()
		{
			auto view = m_Registry.view<T>();
			return view.size() > 0 ? new Entity(view.front(), this) : nullptr;
		}

		Entity* GetMainCamera();
		Entity* FindEntity(const std::string& name);

		void OnUpdateEditor(Transform& cameraTransform, Camera& camera, std::unordered_map<entt::entity, bool>& visible);

		void OnAwakeRuntime();
		void OnUpdateRuntime(bool running, bool paused = false);
		void OnShutdownRuntime();
	private:
		std::string m_Name;
		std::string m_ProjectPath;
		entt::registry m_Registry;

		bool playing = true;

		friend class Entity;
		friend class SceneSerializer;

		// should not befriend classes outside engine lib, fix later
		friend class HierarchyPanel;
		friend class EditorLayer;

		Shader m_GridShader;

		ScriptingEngine* m_ScriptingEngine;
		AssetHandler* m_AssetHandler;
	};
}