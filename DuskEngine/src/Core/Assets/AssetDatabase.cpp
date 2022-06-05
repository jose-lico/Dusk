#include "pch.h"
#include "AssetDatabase.h"

#include "Core/Assets/Assets/Shader.h"
#include "Core/Assets/Assets/Texture.h"
#include "Core/Assets/Assets/Material.h"
#include "Core/Assets/Assets/Model.h"
#include "Core/Assets/AssetHandler.h"

#include "Utils/Serialization/Yaml.h"

#include "uuid.h"

namespace DuskEngine
{
	std::filesystem::path AssetDatabase::m_RootDirectory = "res";
	std::filesystem::path AssetDatabase::m_CurrentDirectory;

	std::unordered_map<uuids::uuid, std::filesystem::path> AssetDatabase::m_PathsMap;
	std::unordered_map <std::filesystem::path, uuids::uuid, opt_path_hash> AssetDatabase::m_UUIDsMap;

	std::vector<Asset*> AssetDatabase::ShaderDatabase;
	std::vector<Asset*> AssetDatabase::ModelDatabase;
	std::vector<Asset*> AssetDatabase::MaterialDatabase;
	std::vector<Asset*> AssetDatabase::ScriptsDatabase;

	void AssetDatabase::Init()
	{
		m_CurrentDirectory = m_RootDirectory;
	}

	void AssetDatabase::Shutdown()
	{
		for (Asset* resource : ShaderDatabase)
			delete resource;

		for (Asset* resource : ModelDatabase)
			delete resource;

		for (Asset* resource : MaterialDatabase)
			delete resource;

		for (Asset* resource : ScriptsDatabase)
			delete resource;
	}

	void AssetDatabase::LoadUUIDs()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (directoryEntry.is_directory())
			{
				m_CurrentDirectory = directoryEntry.path();
				LoadUUIDs();
			}
			else
			{
				if (directoryEntry.path().extension() == ".meta")
				{
					std::ifstream stream(directoryEntry.path());
					std::stringstream strStream;
					strStream << stream.rdbuf();
					
					YAML::Emitter out;
					YAML::Node data = YAML::Load(strStream.str());

#ifdef DUSK_WINDOWS
					std::string path = m_CurrentDirectory.string() + "\\" + directoryEntry.path().stem().string();
#elif DUSK_LINUX
					std::string path = m_CurrentDirectory.string() + "/" + directoryEntry.path().stem().string();
#endif
					uuids::uuid uuid = data["uuid"].as<uuids::uuid>();
					
					//quick fix for name editing (need to remove old entry still)
					if (m_UUIDsMap.find(path) == m_UUIDsMap.end())
					{
						m_PathsMap[uuid] = path;
						m_UUIDsMap[path] = uuid;

						AddToAssetDatabase(path, uuid);
					}
				}
			}
		}

		m_CurrentDirectory = m_RootDirectory;
	}

	void AssetDatabase::CreateUUIDs()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			/*if (directoryEntry.path().filename() == "editor")
				continue;*/

			if(directoryEntry.is_directory())
			{
				m_CurrentDirectory = directoryEntry.path();
				CreateUUIDs();
			}
			else
			{
				std::filesystem::path metaFile = directoryEntry.path().string() + ".meta";

				if(!std::filesystem::exists(metaFile) && directoryEntry.path().extension() != ".meta")
				{
					std::string message = "Creating meta file for " + directoryEntry.path().filename().string();
					LOG(message.c_str());

					uuids::uuid const id = uuids::uuid_system_generator{}();

					YAML::Emitter out;
					out << YAML::BeginMap;
					out << YAML::Key << "uuid" << YAML::Value << id;

					std::string metaName = directoryEntry.path().string() + ".meta";
					std::ofstream fout(metaName.c_str());
					fout << out.c_str();

					m_PathsMap[id] = directoryEntry.path();
				}
			}
		}

		m_CurrentDirectory = m_RootDirectory;
	}

	uuids::uuid AssetDatabase::CreateResource(const std::filesystem::path& path)
	{
		std::string message = "Creating meta file for " + path.string();
		LOG(message.c_str());

		uuids::uuid const id = uuids::uuid_system_generator{}();
		LOG(uuids::to_string(id).c_str());

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "uuid" << YAML::Value << id;

		std::string metaName = path.string() + ".meta";
		std::ofstream fout(metaName.c_str());
		fout << out.c_str();

		m_PathsMap[id] = path;
		m_UUIDsMap[path] = id;

		AddToAssetDatabase(path, id);

		return id;
	}

	void AssetDatabase::AddToAssetDatabase(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		Asset* resource = new Asset(path, uuid);

		std::string extension = path.extension().string();
		
		bool wasAssigned = false;
		
		if (extension == ".glsl")
		{
			wasAssigned = true;
			ShaderDatabase.push_back(resource);
		}
		else if(extension == ".fbx" || extension == ".obj")
		{
			wasAssigned = true;
			ModelDatabase.push_back(resource);
		}
		else if (extension == ".material")
		{
			wasAssigned = true;
			MaterialDatabase.push_back(resource);
		}
		else if (extension == ".lua")
		{
			wasAssigned = true;
			ScriptsDatabase.push_back(resource);
		}
			
		if (!wasAssigned)
			delete resource;
	}

	// If a file is no longer present, delete its meta file
	void AssetDatabase::DeleteUUIDs()
	{
	}

	uuids::uuid AssetDatabase::GetUUID(const std::filesystem::path& path)
	{
		return m_UUIDsMap[path];
	}

	Ref<Material> AssetDatabase::LoadMaterial(const uuids::uuid& uuid, AssetHandler* handler)
	{
		std::ifstream stream(m_PathsMap[uuid]);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		// dumb af
		auto shaderHandle = handler->AddToAssetPool<Shader>(data["Shader"].as<uuids::uuid>());

		Ref<Material> material = MakeRef<Material>(shaderHandle, handler, m_PathsMap[uuid], uuid);

		for (auto& uniform : material->m_Uniforms)
		{
			switch (uniform.Type)
			{
			case UniformType::Vec3:
				uniform.Data.vec3 = data["Uniforms"][uniform.Name].as<glm::vec3>();
				break;
			case UniformType::Texture:
				uniform.Data.dataHandle = handler->AddToAssetPool<Texture>(data["Uniforms"][uniform.Name].as<uuids::uuid>());
				break;
			}
		}

		return material;
	}

	Ref<Shader> AssetDatabase::LoadShader(const uuids::uuid& uuid)
	{
		return Shader::Create(m_PathsMap[uuid], uuid);
	}

	Ref<Texture> AssetDatabase::LoadTexture(const uuids::uuid& uuid)
	{
		return Texture::Create(m_PathsMap[uuid], uuid);
	}

	//TODO - improve model and mesh api and useflow
	Ref<Mesh> AssetDatabase::LoadModel(const uuids::uuid& uuid)
	{
		Model* m = new Model(m_PathsMap[uuid].string(), uuid);
		Ref<Mesh> mesh = m->GetFirstMesh();
		mesh->m_Type = MeshType::Model;
		mesh->m_UUID = uuid;
		mesh->m_Name = m->GetName();
		mesh->m_Path = m->GetPath();
		delete(m);
		return mesh;
	}

	Ref<LuaScript> AssetDatabase::LoadScript(const uuids::uuid& uuid)
	{
		return MakeRef<LuaScript>(m_PathsMap[uuid], uuid);
	}
}