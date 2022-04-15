#pragma once

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