#include "pch.h"
#include "AssetManager.h"

#include "Core/Macros/LOG.h"

#include "Core/Assets/Assets/Shader.h"
#include "Core/Assets/Assets/Texture.h"
#include "Core/Assets/Assets/Material.h"
#include "Core/Assets/Assets/Model.h"
#include "Core/Assets/AssetHandler.h"

#include "Utils/Serialization/Yaml.h"

#include "uuid.h"

namespace DuskEngine
{
	std::filesystem::path AssetManager::m_RootDirectory = "res";
	std::filesystem::path AssetManager::m_CurrentDirectory;

	std::unordered_map<uuids::uuid, std::filesystem::path> AssetManager::m_PathsMap;
	std::unordered_map <std::filesystem::path, uuids::uuid, opt_path_hash> AssetManager::m_UUIDsMap;

	std::vector<Asset*> AssetManager::ShaderList;
	std::vector<Asset*> AssetManager::ModelList;
	std::vector<Asset*> AssetManager::MaterialList;
	std::vector<Asset*> AssetManager::ScriptsList;

	void AssetManager::Init()
	{
		m_CurrentDirectory = m_RootDirectory;
	}

	void AssetManager::Shutdown()
	{
		for (Asset* resource : ShaderList)
			delete resource;

		for (Asset* resource : ModelList)
			delete resource;

		for (Asset* resource : MaterialList)
			delete resource;

		for (Asset* resource : ScriptsList)
			delete resource;
	}

	void AssetManager::LoadUUIDs()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (directoryEntry.path().filename() == "editor")
				continue;

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
					m_PathsMap[uuid] = path;
					m_UUIDsMap[path] = uuid;

					AddToResourceList(path, uuid);
				}
			}
		}

		m_CurrentDirectory = m_RootDirectory;
	}

	void AssetManager::CreateUUIDs()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (directoryEntry.path().filename() == "editor")
				continue;

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
					LOG(message)

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

	uuids::uuid AssetManager::CreateResource(const std::filesystem::path& path)
	{
		std::string message = "Creating meta file for " + path.string();
		LOG(message)

		uuids::uuid const id = uuids::uuid_system_generator{}();
		LOG(uuids::to_string(id))

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "uuid" << YAML::Value << id;

		std::string metaName = path.string() + ".meta";
		std::ofstream fout(metaName.c_str());
		fout << out.c_str();

		m_PathsMap[id] = path;
		m_UUIDsMap[path] = id;

		AddToResourceList(path, id);

		return id;
	}

	void AssetManager::AddToResourceList(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		Asset* resource = new Asset(path, uuid);

		std::string extension = path.extension().string();
		
		bool wasAssigned = false;
		
		if (extension == ".glsl")
		{
			wasAssigned = true;
			ShaderList.push_back(resource);
		}
		else if(extension == ".fbx" || extension == ".obj")
		{
			wasAssigned = true;
			ModelList.push_back(resource);
		}
		else if (extension == ".material")
		{
			wasAssigned = true;
			MaterialList.push_back(resource);
		}
		else if (extension == ".lua")
		{
			wasAssigned = true;
			ScriptsList.push_back(resource);
		}
			
		if (!wasAssigned)
			delete resource;
	}

	// If a file is no longer present, delete its meta file
	void AssetManager::DeleteUUIDs()
	{
	}

	uuids::uuid AssetManager::GetUUID(const std::filesystem::path& path)
	{
		return m_UUIDsMap[path];
	}

	Ref<Material> AssetManager::LoadMaterial(const uuids::uuid& uuid, AssetHandler* handler)
	{
		std::ifstream stream(m_PathsMap[uuid]);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		auto shader = LoadShader(data["Shader"].as<uuids::uuid>());

		Ref<Material> material = MakeRef<Material>(shader, m_PathsMap[uuid], uuid);

		for (auto& uniform : material->m_Uniforms)
		{
			switch (uniform.Type)
			{
			case UniformType::Vec3:
				uniform.Data.vec3 = data["Uniforms"][uniform.Name].as<glm::vec3>();
				break;
			case UniformType::Texture:
				// dumb af
				handler->AddToTexturePool(data["Uniforms"][uniform.Name].as<uuids::uuid>());
				uniform.Data.dataHandle = data["Uniforms"][uniform.Name].as<uuids::uuid>();
				break;
			}
		}

		return material;
	}

	Ref<Shader> AssetManager::LoadShader(const uuids::uuid& uuid)
	{
		return Shader::Create(m_PathsMap[uuid], uuid);
	}

	Ref<Texture> AssetManager::LoadTexture(const uuids::uuid& uuid)
	{
		return Texture::Create(m_PathsMap[uuid], uuid);
	}

	//TODO - improve model and mesh api and useflow
	Ref<Mesh> AssetManager::LoadModel(const uuids::uuid& uuid)
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

	Ref<LuaScript> AssetManager::LoadScript(const uuids::uuid& uuid)
	{
		return MakeRef<LuaScript>(m_PathsMap[uuid], uuid);
	}
}