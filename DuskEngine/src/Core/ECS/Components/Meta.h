#pragma once

#include "uuid.h"

#include <string>

namespace DuskEngine
{
	struct Meta
	{
		bool enabled;
		std::string name;
		uuids::uuid entityHandle;

		Meta(const std::string& name)
			:name(name), enabled(true) {}
	};
}