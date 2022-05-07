#pragma once

#include "Core/Resources/Resource.h"

namespace DuskEngine
{
	class LuaScript : public Resource
	{
	public:
		LuaScript(const std::filesystem::path& path, const uuids::uuid& uuid);
		~LuaScript();

		int GetScriptingID() const { return m_LuaID; }
	private:
		int m_LuaID;
	};
}