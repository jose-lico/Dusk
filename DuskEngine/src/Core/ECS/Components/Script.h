#pragma once

#include "Core/Assets/Handle.h"

namespace DuskEngine
{
	class LuaScript;

	struct Script
	{
		Handle<LuaScript> luaScriptHandle;

		Script() = default;
	};
}