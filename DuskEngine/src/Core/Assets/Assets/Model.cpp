#include "pch.h"
#include "Model.h"

#include "Mesh.h"

#include "glm/glm.hpp"
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

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

	Model::Model(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		UUID = uuid;
		Path = path;
		Name = path.filename().string();

		if(path.extension().string() == ".glb")
		{
			cgltf_options options{ cgltf_file_type_glb };
			cgltf_data* data = NULL;
			cgltf_result result = cgltf_parse_file(&options, path.string().c_str(), &data);
			if (result == cgltf_result_success)
			{
				TRACE("Meshes count: " + std::to_string(data->meshes_count));
				TRACE("Materials count: " + std::to_string(data->materials_count));
				TRACE("Buffers count: " + std::to_string(data->buffers_count));
				TRACE("Images count: " + std::to_string(data->images_count));
				TRACE("Textures count: " + std::to_string(data->textures_count));

				result = cgltf_load_buffers(&options, data, path.string().c_str());

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
						else if(attribute->component_type == cgltf_component_type_r_32u)
						{
							indicesTypeSize = sizeof(unsigned int);
							indicesSize = attribute->count * indicesTypeSize;
							indices = malloc(indicesSize);
							LOAD_ATTRIBUTE(attribute, 1, unsigned int, indices)
						}
						else
							ERR("Indices data format not supported");

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

						m_Meshes.push_back(MakeUnique<Mesh>(vertices, indices, indicesSize, indicesTypeSize));
					}
				}

				cgltf_free(data);
			}
		}
	}

	Model::~Model()
	{
	}
}