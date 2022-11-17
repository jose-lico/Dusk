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
		~ScriptingEngine() = default;

		void LoadScript(LuaScript& script);

		void OnAwake(LuaScript& script);
		void OnUpdate(LuaScript& script);
		void OnShutdown(LuaScript& script);
	private:
		void RegisterFunctions();

		void RegisterInput();
		void RegisterTime();
		void RegisterMath();
		void RegisterECS();

		sol::state m_State;
	};
}