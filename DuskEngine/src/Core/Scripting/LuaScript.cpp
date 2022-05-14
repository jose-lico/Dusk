#include "pch.h"

#include "Core/Macros/LOG.h"
#include "LuaScript.h"

#include "ScriptingEngine.h"

namespace DuskEngine
{
	LuaScript::LuaScript(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		m_UUID = uuid;
		m_Path = path;
		m_Name = path.filename().string();
	}

	LuaScript::~LuaScript()
	{
	}

	void LuaScript::LoadScript(sol::state& state)
	{
		m_Env = sol::environment(state, sol::create, state.globals());

		sol::protected_function loadScript = state.script_file(m_Path.string(), m_Env);
		if(!loadScript.valid())
		{
			// Error Handling
		}

		m_AwakeFunc = m_Env["OnAwake"];
		m_UpdateFunc = m_Env["OnUpdate"];
		m_ShutdownFunc = m_Env["OnShutdown"];

		state.collect_garbage();
	}

	void LuaScript::OnAwake()
	{
		if(m_AwakeFunc.valid())
			m_AwakeFunc.call();
	}

	void LuaScript::OnUpdate()
	{
		if(m_UpdateFunc.valid())
			m_UpdateFunc.call();
	}

	void LuaScript::OnShutdown()
	{
		if(m_ShutdownFunc.valid())
			m_ShutdownFunc.call();
	}
}