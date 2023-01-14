#include "pch.h"
#include "Model.h"

#include "Mesh.h"
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

			ModelDataInternal modelData;
			memcpy(&modelData, &buffer.str()[0], sizeof(ModelDataInternal));

			size_t maxSize = modelData.PositionSize + modelData.NormalsSize + modelData.TextCoordsSize + modelData.IndicesSize;

			float* data = (float*)malloc(maxSize);
			Decompress(data, maxSize, &buffer.str()[0] + sizeof(ModelDataInternal), modelData.CompressedSize);

			size_t verticesSize = modelData.PositionSize + modelData.NormalsSize + modelData.TextCoordsSize;
			std::vector<Vertex> vertices (verticesSize/sizeof(Vertex));

			for (size_t i = 0; i < verticesSize/sizeof(Vertex); i++)
			{
				vertices[i].Position.x = data[i];
				vertices[i].Position.y = data[i + 1 * 3];
				vertices[i].Position.z = data[i + 2 * 3];
			}

			void* indices;

			indices = malloc(modelData.IndicesSize);
			memcpy(indices, (char*)data + modelData.PositionSize + modelData.NormalsSize + modelData.TextCoordsSize , modelData.IndicesSize);
	
			m_Meshes.push_back(MakeUnique<Mesh>(vertices, indices, modelData.IndicesSize, modelData.IndicesTypeSize));

			free(data);
			free(indices);

			importFile.close();
		}
	}

	Model::~Model()
	{
	}
}