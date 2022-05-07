#pragma once

#include "Utils/Memory/Memory.h"

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

		static lua_State* m_LuaState;
	};
}