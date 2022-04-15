#pragma once

#include "visit_struct/visit_struct.hpp"

#include <string>

namespace DuskEngine
{
	struct Meta
	{
		std::string name;
		bool enabled;

		Meta() = default;
		Meta(const std::string& name)
			:name(name), enabled(true) {}
	};
}

VISITABLE_STRUCT(DuskEngine::Meta, name, enabled);