#include "ModelImporter.h"

#include "Core/Application/Core.h"
#include "Core/Application/Application.h"
#include "Core/Assets/Assets/Mesh.h"
#include "Utils/Compression/Compression.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#include <fstream>

namespace DuskEngine
{
	// Implementation based on raylib https://github.com/raysan5/raylib/blob/master/src/rmodels.c

#define LOAD_ATTRIBUTE(attribute, elementCount, type, destination) \
{ \
	int n = 0; \
	type* buffer = (type*)attribute->buffer_view->buffer->data + attribute->buffer_view->offset/sizeof(type) + attribute->offset/sizeof(type); \
	for (unsigned int k = 0; k < attribute->count; k++) \
    {\
		for (int l = 0; l < elementCount; l++) \
		{\
			((type*)destination)[elementCount * k + l] = buffer[n + l];\
		}\
		n += (int)(attribute->stride/sizeof(type));\
		}\
}

	void ImportModel(const std::filesystem::path& modelPath, const std::filesystem::path& importFilePath)
	{
		cgltf_options options{ cgltf_file_type_glb };
		cgltf_data* data = NULL;
		cgltf_result result = cgltf_parse_file(&options, modelPath.string().c_str(), &data);
		
		if (result == cgltf_result_success)
		{
			result = cgltf_load_buffers(&options, data, modelPath.string().c_str());

			//for (unsigned int i = 0; i < data->meshes_count; i++)
			for (unsigned int i = 0; i < 1; i++) // What is the difference between mesh and primitive in cgltf?
			{
				for (unsigned int p = 0; p < data->meshes[i].primitives_count; p++)
				{
					std::vector<float> positions;
					std::vector<float> normals;
					std::vector<float> textureCoords;

					for (unsigned int j = 0; j < data->meshes[i].primitives[p].attributes_count; j++)
					{
						if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_position)      // POSITION
						{
							cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;

							if ((attribute->component_type == cgltf_component_type_r_32f) && (attribute->type == cgltf_type_vec3))
							{
								positions.resize(attribute->count * 3);
								LOAD_ATTRIBUTE(attribute, 3, float, positions.data())
							}
							else
								ERR("Vertex Position data format not supported");
						}
						else if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_normal)   // NORMAL
						{
							cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;

							if ((attribute->component_type == cgltf_component_type_r_32f) && (attribute->type == cgltf_type_vec3))
							{
								normals.resize(attribute->count * 3);
								LOAD_ATTRIBUTE(attribute, 3, float, normals.data())
							}
							else
								ERR("Normal data format not supported");
						}
						else if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_texcoord) // TEXCOORD_0
						{
							cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;

							if ((attribute->component_type == cgltf_component_type_r_32f) && (attribute->type == cgltf_type_vec2))
							{
								textureCoords.resize(attribute->count * 2);
								LOAD_ATTRIBUTE(attribute, 2, float, textureCoords.data())
							}
							else
								ERR("Texture Coords data format not supported");
						}
					}

					cgltf_accessor* attribute = data->meshes[i].primitives[p].indices;

					void* indices;
					size_t indicesTypeSize;
					size_t indicesSize;

					if (attribute->component_type == cgltf_component_type_r_16u)
					{
						indicesTypeSize = sizeof(unsigned short);
						indicesSize = attribute->count * indicesTypeSize;
						indices = malloc(indicesSize);
						LOAD_ATTRIBUTE(attribute, 1, unsigned short, indices)
					}
					else if (attribute->component_type == cgltf_component_type_r_32u)
					{
						indicesTypeSize = sizeof(unsigned int);
						indicesSize = attribute->count * indicesTypeSize;
						indices = malloc(indicesSize);
						LOAD_ATTRIBUTE(attribute, 1, unsigned int, indices)
					}
					else
						ERR("Indices data format not supported");

					std::filesystem::create_directory(Application::Get().GetProjectPath() / ".import");
					std::filesystem::create_directory(Application::Get().GetProjectPath() / ".import/models");

					ModelData modelData;
					modelData.PositionSize = positions.size() * sizeof(float) * 3;
					modelData.NormalsSize = normals.size() * sizeof(float) * 3;
					modelData.TextCoordsSize = positions.size() * sizeof(float) * 2;
					modelData.IndicesSize = indicesSize;
					modelData.IndicesTypeSize = indicesTypeSize;

					size_t modelDataSize = modelData.PositionSize + modelData.NormalsSize + modelData.TextCoordsSize + modelData.IndicesSize;

					void* compressedData = malloc(modelDataSize);
					void* dataToCompress = malloc(modelDataSize);

					memcpy((char*)dataToCompress, positions.data(), modelData.PositionSize);
					memcpy((char*)dataToCompress + modelData.PositionSize, normals.data(), modelData.NormalsSize);
					memcpy((char*)dataToCompress + modelData.PositionSize + modelData.NormalsSize, textureCoords.data(), modelData.TextCoordsSize);
					memcpy((char*)dataToCompress + modelData.PositionSize + modelData.NormalsSize + modelData.TextCoordsSize, indices, modelData.IndicesSize);

					modelData.CompressedSize = Compress(compressedData, dataToCompress, modelDataSize);

					size_t dataSize = sizeof(ModelData) + modelData.CompressedSize;

					void* importedFileData = malloc(dataSize);
					memcpy(importedFileData, &modelData, sizeof(ModelData));
					memcpy((char*)importedFileData + sizeof(ModelData), compressedData, modelData.CompressedSize);

					std::ofstream importFile(importFilePath, std::ios::app | std::ios::binary);
					importFile.write((char*)importedFileData, dataSize);

					free(importedFileData);
					free(dataToCompress);
					free(compressedData);
				}
			}
		}
		
		cgltf_free(data);
	}
}