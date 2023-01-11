#pragma once

#include "glm/glm.hpp"

#include <filesystem>

namespace DuskEngine
{
	struct ModelData
	{
		size_t PositionSize;
		size_t NormalsSize;
		size_t TextCoordsSize;
		size_t IndicesSize;
		size_t IndicesTypeSize;
	};

	void ImportModel(const std::filesystem::path& modelPath, const std::filesystem::path& importFilePath);
}