#pragma once

#include <string>

namespace DuskEngine
{
	struct Meta
	{
		std::string Name;

		Meta() = default;
		Meta(const std::string& name)
			:Name(name) {}
	};
}