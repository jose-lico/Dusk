#include "pch.h"
#include "LuaScript.h"

#include "ScriptingEngine.h"

namespace DuskEngine
{
	LuaScript::LuaScript(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		m_UUID = uuid;
		m_Path = path;
		m_Name = path.filename().string();

		m_LuaID = ScriptingEngine::LoadScript(m_Path);
	}

	LuaScript::~LuaScript()
	{
	}
}