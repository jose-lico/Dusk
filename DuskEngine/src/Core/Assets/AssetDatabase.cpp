#include "pch.h"
#include "AssetDatabase.h"

#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/Texture.h"
#include "Core/Assets/Assets/Material.h"
#include "Core/Assets/Assets/Model.h"
#include "Core/Assets/AssetHandler.h"

#include "Utils/Serialization/Yaml.h"

#include "uuid.h"

#undef CreateMetaFile

namespace DuskEngine
{
	AssetDatabase::AssetDatabase()
	{
		m_CurrentDirectory = m_RootDirectory;
	}

	AssetDatabase::~AssetDatabase()
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

	void AssetDatabase::CreateUUIDs()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (directoryEntry.path().filename() == "import")
				continue;

			if(directoryEntry.is_directory())
			{
				m_CurrentDirectory = directoryEntry.path();
				CreateUUIDs();
			}
			else
			{
				// If the file is a meta file, register it
				if (directoryEntry.path().extension() == ".meta")
				{
					RegisterAsset(directoryEntry);
				}
				// If the file does not have a meta file and is not a meta file itself
				else if (!std::filesystem::exists(directoryEntry.path().string() + ".meta") && directoryEntry.path().extension() != ".meta")
				{
					CreateMetaFile(directoryEntry);
				}
			}
		}

		m_CurrentDirectory = m_RootDirectory;
	}

	void AssetDatabase::CreateMetaFile(const std::filesystem::directory_entry& directoryEntry)
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
		fout.close();

		m_PathsMap[id] = directoryEntry.path();
	}

	void AssetDatabase::RegisterAsset(const std::filesystem::directory_entry& directoryEntry)
	{
		// If the file has been moved, removed or renamed, delete meta file.
		if(!std::filesystem::exists(m_CurrentDirectory / directoryEntry.path().stem()))
		{
			std::string message = directoryEntry.path().stem().string() + " has been moved, removed or renamed, deleting meta file.";
			WARN(message.c_str());
			std::filesystem::remove(directoryEntry);
			return;
		}

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
		fout.close();

		m_PathsMap[id] = path;
		m_UUIDsMap[path] = id;

		AddToAssetDatabase(path, id);

		return id;
	}

	// 1. Add asset to asset database of correct type
	// 2. Import asset if it has not been imported yet
	void AssetDatabase::AddToAssetDatabase(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		Asset* resource = new Asset(path, uuid);

		std::string extension = path.extension().string();
		
		bool wasAssigned = false;
		
		if(extension == ".png" || extension == ".jpg")
		{
			std::string importFile = "res/import/" + path.filename().string() + "-" + uuids::to_string(uuid) + ".import";
			if(!std::filesystem::exists(importFile))
			{
				std::string message = "Importing " + path.string();
				TRACE(message.c_str());

				std::ofstream fout(importFile, std::ios::app | std::ios::binary);
				Texture::ImportTest(path, fout);
				fout.close();
			}
		}
		else if (extension == ".glsl")
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

	uuids::uuid AssetDatabase::GetUUID(const std::filesystem::path& path)
	{
		return m_UUIDsMap[path];
	}

	UniqueRef<Material> AssetDatabase::LoadMaterial(const uuids::uuid& uuid, AssetHandler* handler)
	{
		std::ifstream stream(m_PathsMap[uuid]);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		// dumb af
		auto shaderHandle = handler->AddToAssetPool<Shader>(data["Shader"].as<uuids::uuid>());

		UniqueRef<Material> material = MakeUnique<Material>(shaderHandle, handler, m_PathsMap[uuid], uuid);

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

	UniqueRef<Shader> AssetDatabase::LoadShader(const uuids::uuid& uuid)
	{
		return MakeUnique<Shader>(m_PathsMap[uuid], uuid);
	}

	UniqueRef<Texture> AssetDatabase::LoadTexture(const uuids::uuid& uuid)
	{
		return MakeUnique<Texture>(m_PathsMap[uuid], uuid);
	}

	//TODO - improve model and mesh api and useflow
	UniqueRef<Mesh> AssetDatabase::LoadModel(const uuids::uuid& uuid)
	{
		Model* m = new Model(m_PathsMap[uuid].string(), uuid);
		UniqueRef<Mesh> mesh = std::move(m->GetFirstMesh());
		mesh->m_Type = MeshType::Model;
		mesh->m_UUID = uuid;
		mesh->m_Name = m->GetName();
		mesh->m_Path = m->GetPath();
		delete(m);
		return mesh;
	}

	UniqueRef<LuaScript> AssetDatabase::LoadScript(const uuids::uuid& uuid)
	{
		return MakeUnique<LuaScript>(m_PathsMap[uuid], uuid);
	}
}