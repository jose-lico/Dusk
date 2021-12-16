#pragma once

#include <string>

#include <visit_struct/visit_struct.hpp>

namespace DuskEngine
{
	struct Meta
	{
		std::string name;

		Meta() = default;
		Meta(const std::string& name)
			:name(name) {}
	};
}

VISITABLE_STRUCT(DuskEngine::Meta, name);