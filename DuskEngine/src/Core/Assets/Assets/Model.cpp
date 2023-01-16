#include "pch.h"
#include "Model.h"

#include "Core/Application/Application.h"
#include "Utils/Compression/Compression.h"

#include "glm/glm.hpp"

namespace DuskEngine
{
	Model::Model(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		UUID = uuid;
		Path = path;
		Name = path.filename().string();

		std::filesystem::path importFilePath = (std::filesystem::path)Application::Get().GetProjectPath() / (std::filesystem::path)(".import/models/" + path.filename().string() + "-" + uuids::to_string(uuid) + ".import");

		if (std::filesystem::exists(importFilePath))
		{
			std::ifstream importFile(importFilePath, std::ios::binary);
			std::stringstream buffer;
			buffer << importFile.rdbuf();

			ModelData modelData;
			memcpy(&modelData, &buffer.str()[0], sizeof(ModelData));

			uint32_t vertSize = modelData.VertCount * sizeof(Vertex);
			uint32_t maxSize = vertSize + modelData.IndicesSize;

			void* data = malloc(maxSize);
			Decompress(data, maxSize, &buffer.str()[0] + sizeof(ModelData), modelData.CompressedSize);

			Vertex* vertices = (Vertex*)malloc(vertSize);
			memcpy(vertices, (char*)data, vertSize);

			void* indices = malloc(modelData.IndicesSize);
			memcpy(indices, (char*)data + vertSize, modelData.IndicesSize);
	
			m_Meshes.push_back(MakeUnique<Mesh>(vertices, vertSize, indices, modelData.IndicesSize, modelData.IndicesTypeSize));

			free(data);
			free(indices);

			importFile.close();
		}
	}

	Model::~Model()
	{
	}
}