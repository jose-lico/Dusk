#pragma once

#include "Core/Scripting/LuaScript.h"
#include "Utils/Memory/Memory.h"

#include <vector>

namespace DuskEngine
{
	struct Script
	{
		std::vector<Ref<LuaScript>> scripts;

		Script() = default;

		Entity ent;
	};
}