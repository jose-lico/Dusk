#pragma once

#include <filesystem>

namespace DuskEngine :: Filesystem
{
	/// <summary>
	/// Creates a directory if it does not exist
	/// </summary>
	/// <param name="dirPath"></param>
	static void CreateDir(const std::filesystem::path& dirPath);
}
