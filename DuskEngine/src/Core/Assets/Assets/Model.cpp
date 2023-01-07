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
	/*Model::Model(const std::string& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}

		ProcessNode(scene->mRootNode, scene);
	}*/

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
						std::vector<glm::vec3> normals;
						std::vector<glm::vec2> textureCoords;
						for (unsigned int j = 0; j < data->meshes[i].primitives[p].attributes_count; j++)
						{
							if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_position)      // POSITION
							{
								TRACE("Position Data");
								cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;
								TRACE("Vertex Count: " + std::to_string(attribute->count));
								positions.resize(attribute->count * 3);
								TRACE("Positions size:" + std::to_string(positions.size()));
								//memcpy(&positions[0], attribute->buffer_view->buffer->data, attribute->count);

								int n = 0;
								float* buffer = (float*)attribute->buffer_view->buffer->data + attribute->buffer_view->offset / sizeof(float) + attribute->offset / sizeof(float);
								for (unsigned int k = 0; k < attribute->count; k++)
								{
									for (int l = 0; l < 3; l++) \
									{
										positions[3 * k + l] = buffer[n + l];
									}

									n += (int)(attribute->stride / sizeof(float));
								}
							}
							else if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_normal)   // NORMAL
							{
								TRACE("Normal Data");
								cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;
								TRACE("Normals Count: " + std::to_string(attribute->count));
								normals.resize(attribute->count);
								memcpy(&normals[0], attribute->buffer_view->buffer->data, attribute->count);
							}
							else if (data->meshes[i].primitives[p].attributes[j].type == cgltf_attribute_type_texcoord) // TEXCOORD_0
							{
								TRACE("Text Coords Data");
								cgltf_accessor* attribute = data->meshes[i].primitives[p].attributes[j].data;
								TRACE("Text Coords Count: " + std::to_string(attribute->count));
								textureCoords.resize(attribute->count);
								memcpy(&textureCoords[0], attribute->buffer_view->buffer->data, attribute->count);
							}
						}

						std::vector<Vertex> vertices;
						//vertices.resize(normals.size());
						for (size_t x = 0; x < normals.size(); x++)
						{
							Vertex vertex;
							vertex.Position.x = positions[x * 3];
							vertex.Position.y = positions[x * 3 + 1];
							vertex.Position.z = positions[x * 3 + 2];
							vertex.Normal = normals[x];
							vertex.TexCoords = textureCoords[x];
							vertices.push_back(vertex);
						}

						TRACE("Positions size:" + std::to_string(positions.size()));

						std::vector<unsigned int> indices;
						cgltf_accessor* attribute = data->meshes[i].primitives[p].indices;
						indices.resize(attribute->count);
						memcpy(&indices[0], attribute->buffer_view->buffer->data, attribute->count);

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