#pragma once

#include "Utils/Memory/Memory.h"

class lua_State;

namespace DuskEngine
{
	class LuaScript;

	class ScriptingEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnAwake(Ref<LuaScript>& script);
	private:
		static lua_State* m_LuaState;
	};
}