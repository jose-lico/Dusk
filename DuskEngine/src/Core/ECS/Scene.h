#pragma once
#include "Core/Application/Core.h"

#include "entt/entity/registry.hpp"

namespace DuskEngine
{
	class Entity;
	class EditorCamera;
	class Shader;
	class ScriptingEngine;
	class AssetHandler;

	class DUSK_EXPORT Scene
	{
	public:
		Scene(const std::string& name);
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

		void OnUpdateEditor(EditorCamera& camera);

		void OnAwakeRuntime();
		void OnUpdateRuntime(bool running, bool paused = false);
		void OnShutdownRuntime();
	private:
		std::string m_Name;
		entt::registry m_Registry;

		bool playing = true;

		friend class Entity;
		friend class SceneSerializer;

		// should not befriend classes outside engine lib, fix later
		friend class HierarchyPanel;
		friend class EditorLayer;

		Ref<Shader> m_GridShader;

		ScriptingEngine* m_ScriptingEngine;
		AssetHandler* m_AssetHandler;
	};
}