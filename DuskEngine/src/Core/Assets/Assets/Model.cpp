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

			size_t maxSize = modelData.IndicesSize + modelData.PositionSize + modelData.NormalsSize + modelData.TextCoordsSize;

			void* data = malloc(maxSize);
			Decompress(data, maxSize, &buffer.str()[0] + sizeof(ModelDataInternal), modelData.CompressedSize);

			std::vector<float> positions(modelData.PositionSize);
			std::vector<float> normals(modelData.NormalsSize);
			std::vector<float> textureCoords(modelData.TextCoordsSize);

			memcpy(positions.data(), data, modelData.PositionSize);
			memcpy(normals.data(), (char*)data + modelData.PositionSize, modelData.NormalsSize);
			memcpy(textureCoords.data(), (char*)data + modelData.PositionSize + modelData.NormalsSize, modelData.TextCoordsSize);

			std::vector<Vertex> vertices;

			for (size_t x = 0; x < positions.size() / 3; x++)
			{
				Vertex vertex;
				vertex.Position.x = positions[x * 3];
				vertex.Position.y = positions[x * 3 + 1];
				vertex.Position.z = positions[x * 3 + 2];

				vertex.Normal.x = normals[x * 3];
				vertex.Normal.y = normals[x * 3 + 1];
				vertex.Normal.z = normals[x * 3 + 2];

				vertex.TexCoords.x = textureCoords[x * 2];
				vertex.TexCoords.y = 1 - textureCoords[x * 2 + 1];

				vertices.push_back(vertex);
			}

			void* indices;

			indices = malloc(modelData.IndicesSize);
			memcpy(indices, (char*)data + modelData.PositionSize + modelData.NormalsSize + modelData.TextCoordsSize, modelData.IndicesSize);
	
			m_Meshes.push_back(MakeUnique<Mesh>(vertices, indices, modelData.IndicesSize, modelData.IndicesTypeSize));

			free(data);
			importFile.close();
		}
	}

	Model::~Model()
	{
	}
}