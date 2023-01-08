#include "pch.h"
#include "Model.h"

#include "Mesh.h"

#include "glm/glm.hpp"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

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
			destination[elementCount * k + l] = buffer[n + l];\
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

				for (unsigned int i = 0, meshIndex = 0; i < data->meshes_count; i++)
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
								positions.resize(attribute->count * 3);
								LOAD_ATTRIBUTE(attribute, 3, float, positions.data())
							}
							else if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_normal)   // NORMAL
							{
								cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;
								normals.resize(attribute->count * 3);
								LOAD_ATTRIBUTE(attribute, 3, float, normals.data())
							}
							else if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_texcoord) // TEXCOORD_0
							{
								cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;
								textureCoords.resize(attribute->count * 2);
								LOAD_ATTRIBUTE(attribute, 2, float, textureCoords.data())
							}
						}

						std::vector<Vertex> vertices;
						//vertices.resize(normals.size());

						TRACE("Positions size:" + std::to_string(positions.size()/3));

						for (size_t x = 0; x < positions.size()/3; x++)
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

						std::vector<unsigned short> indices;
						cgltf_accessor* attribute = data->meshes[i].primitives[p].indices;
						indices.resize(attribute->count);

						LOAD_ATTRIBUTE(attribute, 1, unsigned short, indices.data())
						std::reverse(indices.begin(), indices.end());
						m_Meshes.push_back(MakeUnique<Mesh>(vertices, &indices[0], (unsigned int)indices.size()));
					}
				}

				cgltf_free(data);
			}
		}
		else
		{
			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::string message = "ERROR::ASSIMP::";
				message.append(import.GetErrorString());
				WARN(message.c_str());
				return;
			}

			LOG("Loading Model " + Name);
			ProcessNode(scene->mRootNode, scene);
		}
	}

	Model::~Model()
	{
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	UniqueRef<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			// positions
			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;

			//// texture coordinates
			if (mesh->mTextureCoords[0])
			{
				vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.TexCoords.y = 1 - mesh->mTextureCoords[0][i].y;
			}

			// normals
			if (mesh->HasNormals())
			{
				vertex.Normal.x = mesh->mNormals[i].x;
				vertex.Normal.y = mesh->mNormals[i].y;
				vertex.Normal.z = mesh->mNormals[i].z;
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector

			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return MakeUnique<Mesh>(vertices, &indices[0], (unsigned int)indices.size());
	}
}