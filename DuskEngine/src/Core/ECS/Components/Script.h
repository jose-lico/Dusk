#pragma once

#include "uuid.h"

namespace DuskEngine
{
	struct Script
	{
		uuids::uuid luaScriptHandle;

		Script() = default;
	};
}