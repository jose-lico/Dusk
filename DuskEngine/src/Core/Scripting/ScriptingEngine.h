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

		void LoadScript(Ref<LuaScript>& script);

		void OnAwake(Ref<LuaScript>& script);
		void OnUpdate(Ref<LuaScript>& script);
		void OnShutdown(Ref<LuaScript>& script);
	private:
		void RegisterFunctions();

		void RegisterInput();
		void RegisterTime();
		void RegisterMath();
		void RegisterECS();

		sol::state m_State;
	};
}