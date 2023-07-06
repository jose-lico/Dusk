#include "ModelImporter.h"

#include "Core/Application/Core.h"
#include "Core/Application/Application.h"
#include "Core/Assets/Assets/Model.h"
#include "Utils/Compression/Compression.h"

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

			// What is the difference between mesh and primitive in cgltf?

			WARN("Importing model {} , only first mesh will be imported and used", modelPath.filename().string());

			//for (unsigned int i = 0; i < data->meshes_count; i++)
			for (unsigned int i = 0; i < 1; i++) 
			{
				//for (unsigned int p = 0; p < data->meshes[i].primitives_count; p++)
				for (unsigned int p = 0; p < 1; p++)
				{
					ModelData modelData;

					float* positions = nullptr;
					float* textureCoords = nullptr;
					float* normals= nullptr;

					for (unsigned int j = 0; j < data->meshes[i].primitives[p].attributes_count; j++)
					{
						if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_position)      // POSITION
						{
							cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;

							if ((attribute->component_type == cgltf_component_type_r_32f) && (attribute->type == cgltf_type_vec3))
							{
								modelData.VertCount = attribute->count;
								
								positions = (float*)malloc(attribute->count * sizeof(float) * 3);

								LOAD_ATTRIBUTE(attribute, 3, float, positions)
							}
							else
								ERR("Vertex Position data format not supported");
						}
						else if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_texcoord) // TEXCOORD_0
						{
							cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;

							if ((attribute->component_type == cgltf_component_type_r_32f) && (attribute->type == cgltf_type_vec2))
							{
								textureCoords = (float*)malloc(attribute->count * sizeof(float) * 2);

								LOAD_ATTRIBUTE(attribute, 2, float, textureCoords)
							}
							else
								ERR("Texture Coords data format not supported");
						}
						else if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_normal)   // NORMAL
						{
							cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;

							if ((attribute->component_type == cgltf_component_type_r_32f) && (attribute->type == cgltf_type_vec3))
							{
								normals = (float*)malloc(attribute->count * sizeof(float) * 3);

								LOAD_ATTRIBUTE(attribute, 3, float, normals)
							}
							else
								ERR("Normal data format not supported");
						}
					}

					cgltf_accessor* attribute = data->meshes[i].primitives[p].indices;

					void* indices = nullptr;

					if (attribute->component_type == cgltf_component_type_r_16u)
					{
						modelData.IndicesTypeSize = sizeof(unsigned short);
						modelData.IndicesSize = attribute->count * modelData.IndicesTypeSize;
						indices = malloc(modelData.IndicesSize);
						LOAD_ATTRIBUTE(attribute, 1, unsigned short, indices)
					}
					else if (attribute->component_type == cgltf_component_type_r_32u)
					{
						modelData.IndicesTypeSize = sizeof(unsigned int);
						modelData.IndicesSize = attribute->count * modelData.IndicesTypeSize;
						indices = malloc(modelData.IndicesSize);
						LOAD_ATTRIBUTE(attribute, 1, unsigned int, indices)
					}
					else
						ERR("Indices data format not supported");

					std::filesystem::create_directory(Application::Get().GetProjectPath() / ".import");
					std::filesystem::create_directory(Application::Get().GetProjectPath() / ".import/models");

					// Pack raw data to be compressed
					uint32_t vertSize = modelData.VertCount * sizeof(Vertex);
					void* rawData = malloc(vertSize + modelData.IndicesSize);
					
					// Pack vertices as engine expects them (3 Pos, 2 Tex, 3 Norm)
					for(uint32_t k = 0; k < modelData.VertCount; k++)
					{
						((float*)rawData)[k * 8]		= positions[k * 3];
						((float*)rawData)[k * 8 + 1]	= positions[k * 3 + 1];
						((float*)rawData)[k * 8 + 2]	= positions[k * 3 + 2];

						((float*)rawData)[k * 8 + 3]	= textureCoords[k * 2];
						((float*)rawData)[k * 8 + 4]	= 1 - textureCoords[k * 2 + 1];
						
						((float*)rawData)[k * 8 + 5]	= normals[k * 3];
						((float*)rawData)[k * 8 + 6]	= normals[k * 3 + 1];
						((float*)rawData)[k * 8 + 7]	= normals[k * 3 + 2];
					}

					// Pack indices right after verts
					memcpy((char*)rawData + vertSize, indices, modelData.IndicesSize);

					// Compress model data
					void* compressedData = malloc(vertSize + modelData.IndicesSize);
					modelData.CompressedSize = (uint32_t)Compress(compressedData, rawData, vertSize + modelData.IndicesSize);

					// Write to file
					std::ofstream importFile(importFilePath, std::ios::app | std::ios::binary);
					importFile.write((char*)&modelData, sizeof(ModelData));
					importFile.write((char*)compressedData, modelData.CompressedSize);

					// Free
					free(positions);
					free(normals);
					free(textureCoords);

					free(rawData);
					free(compressedData);
				}
			}
		}
		
		cgltf_free(data);
	}
}