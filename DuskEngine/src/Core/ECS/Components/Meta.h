#pragma once

#include <string>

namespace DuskEngine
{
	struct Meta
	{
		bool enabled;
		std::string name;

		Meta(const std::string& name)
			:name(name), enabled(true) {}
	};
}