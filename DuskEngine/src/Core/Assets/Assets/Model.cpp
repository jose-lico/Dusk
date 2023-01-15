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

			Vertex* data = (Vertex*)malloc(maxSize);
			Decompress(data, maxSize, &buffer.str()[0] + sizeof(ModelDataInternal), modelData.CompressedSize);

			size_t verticesSize = modelData.PositionSize + modelData.NormalsSize + modelData.TextCoordsSize;
			std::vector<Vertex> vertices (11861);

			for (size_t i = 0; i < vertices.size(); i++)
			{
				vertices[i] = data[i];
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