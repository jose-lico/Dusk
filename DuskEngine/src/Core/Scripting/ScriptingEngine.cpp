#include "pch.h"
#include "ScriptingEngine.h"

#include "LuaScript.h"

#include "lua/lua.hpp"

namespace DuskEngine
{
	lua_State* ScriptingEngine::m_LuaState = nullptr;

	void ScriptingEngine::Init()
	{
		m_LuaState = luaL_newstate();
		luaL_openlibs(m_LuaState);
	}

	void ScriptingEngine::Shutdown()
	{
		lua_close(m_LuaState);
	}

	void ScriptingEngine::OnAwake(Ref<LuaScript>& script)
	{
		int r = luaL_dofile(m_LuaState, script->GetPath().string().c_str());

		if (r == LUA_OK)
		{
			//lua_getglobal(L, "sprite");
		}
		else
		{
			std::string error = lua_tostring(m_LuaState, -1);
			std::cout << error << std::endl;
		}
	}
}
