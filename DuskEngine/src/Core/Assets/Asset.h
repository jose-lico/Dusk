#pragma once

#include <uuid.h>
#include <string>
#include <filesystem>

namespace DuskEngine
{
	struct Asset
	{
		std::filesystem::path Path;
		std::string Name;
		uuids::uuid UUID;
	};
}