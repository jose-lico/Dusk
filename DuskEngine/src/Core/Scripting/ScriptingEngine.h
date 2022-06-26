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

		void LoadScript(UniqueRef<LuaScript>& script);

		void OnAwake(UniqueRef<LuaScript>& script);
		void OnUpdate(UniqueRef<LuaScript>& script);
		void OnShutdown(UniqueRef<LuaScript>& script);
	private:
		void RegisterFunctions();

		void RegisterInput();
		void RegisterTime();
		void RegisterMath();
		void RegisterECS();

		sol::state m_State;
	};
}