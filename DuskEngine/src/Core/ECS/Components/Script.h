#pragma once

#include "Core/Assets/Handle.h"

namespace DuskEngine
{
	struct Script
	{
		class LuaScript;
		Handle<LuaScript> luaScriptHandle;

		Script() = default;
	};
}