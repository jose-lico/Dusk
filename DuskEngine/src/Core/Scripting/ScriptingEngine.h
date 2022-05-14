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
		~ScriptingEngine();

		void LoadScript(Ref<LuaScript>& script);

		void OnAwake(Ref<LuaScript>& script);
		void OnUpdate(Ref<LuaScript>& script);
		void OnShutdown(Ref<LuaScript>& script);
	private:
		void RegisterFunctions();

		sol::state m_State;
	};
}