#pragma once

#include "Core/Assets/Asset.h"
#include "Core/ECS/Entity.h"

#include "sol/sol.hpp"

namespace DuskEngine
{
	class LuaScript : public Asset
	{
	public:
		LuaScript(const std::filesystem::path& path, const uuids::uuid& uuid);
		~LuaScript();

		void LoadScript(sol::state& state);

		void OnAwake();
		void OnUpdate();
		void OnShutdown();
		Entity GetEntity() 
		{
			return m_Ent; 
		}
	private:
		Entity m_Ent;

		sol::environment m_Env;

		sol::protected_function m_AwakeFunc;
		sol::protected_function m_UpdateFunc;
		sol::protected_function m_ShutdownFunc;

		friend class SceneSerializer;
	};
}