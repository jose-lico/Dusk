#include "pch.h"
#include "LuaScript.h"

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
}