#pragma once

#include "Utils/Memory/Memory.h"

#include "sol/sol.hpp"

namespace DuskEngine
{
	class LuaScript;

	class ScriptingEngine
	{
	public:
		ScriptingEngine();

		void LoadScript(Ref<LuaScript>& script);

		void OnAwake(Ref<LuaScript>& script);
		void OnUpdate(Ref<LuaScript>& script);
		void OnShutdown(Ref<LuaScript>& script);
	private:
		void RegisterFunctions();

		void RegisterInput();

		sol::state m_State;
	};
}