#pragma once

#include <string>

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