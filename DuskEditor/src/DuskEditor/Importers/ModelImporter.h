#pragma once

#include "glm/glm.hpp"

#include <filesystem>

namespace DuskEngine
{
	struct ModelData
	{
		size_t PositionSize = 0;
		size_t NormalsSize = 0;
		size_t TextCoordsSize = 0;
		size_t IndicesSize = 0;
		size_t IndicesTypeSize = 0;
		size_t CompressedSize = 0;
	};

	void ImportModel(const std::filesystem::path& modelPath, const std::filesystem::path& importFilePath);
}