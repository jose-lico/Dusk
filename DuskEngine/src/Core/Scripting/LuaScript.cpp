#include "pch.h"

#include "LuaScript.h"

#include "ScriptingEngine.h"

namespace DuskEngine
{
	LuaScript::LuaScript(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		UUID = uuid;
		Path = path;
		Name = path.filename().string();
	}

	LuaScript::~LuaScript()
	{
		std::string message = "Destroyed Script " + Name;
		LOG(message.c_str());
	}

	void LuaScript::LoadScript(sol::state& state)
	{
		m_Env = sol::environment(state, sol::create, state.globals());

		m_Env["entity"] = m_Ent;

		auto loadScript = state.script_file(Path.string(), m_Env);
		
		if(!loadScript.valid())
		{
			// Error Handling
			LOG("Error loading script");
		}

		m_AwakeFunc = sol::protected_function(m_Env["OnAwake"]);
		m_UpdateFunc = sol::protected_function(m_Env["OnUpdate"]);
		m_ShutdownFunc = sol::protected_function(m_Env["OnShutdown"]);

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