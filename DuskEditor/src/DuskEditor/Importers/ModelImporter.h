#pragma once

#include <filesystem>

namespace DuskEngine
{
	void ImportModel(const std::filesystem::path& modelPath, const std::filesystem::path& importFilePath);
}