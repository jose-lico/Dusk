#pragma once

#include "Utils/Memory/Memory.h"

#include "sol/sol.hpp"

#include <filesystem>

struct lua_State;

namespace DuskEngine
{
	class LuaScript;

	class ScriptingEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static int LoadScript(const std::filesystem::path& path);

		static void OnAwake(Ref<LuaScript>& script);
	private:
		static void RegisterFunctions();

		static sol::state m_State;
		//static lua_State* m_LuaState;
	};
}