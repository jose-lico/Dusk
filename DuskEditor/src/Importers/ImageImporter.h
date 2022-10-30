#pragma once

#include <filesystem>

namespace DuskEngine
{
	struct ImageData
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Channels;
		uint64_t DataSize;
	};

    void ImportImage(const std::filesystem::path& imagePath, const std::filesystem::path& importFilePath);
}