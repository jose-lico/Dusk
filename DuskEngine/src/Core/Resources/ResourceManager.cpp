#include "pch.h"
#include "ResourceManager.h"

#include "Core/Macros/LOG.h"

#include "Core/Resources/Resources/Shader.h"
#include "Core/Resources/Resources/Texture.h"
#include "Core/Resources/Resources/Material.h"
#include "Core/Resources/Resources/Model.h"

#include "Utils/Serialization/Yaml.h"

#include "uuid.h"

namespace DuskEngine
{
	std::filesystem::path ResourceManager::m_RootDirectory = "res";
	std::filesystem::path ResourceManager::m_CurrentDirectory;

	std::unordered_map<uuids::uuid, std::filesystem::path> ResourceManager::m_PathsMap;
	std::unordered_map <std::filesystem::path, uuids::uuid, opt_path_hash> ResourceManager::m_UUIDsMap;

	void ResourceManager::Init()
	{
		m_CurrentDirectory = m_RootDirectory;
	}

	void ResourceManager::LoadUUIDs()
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

					std::string path = m_CurrentDirectory.string() + "/" + directoryEntry.path().stem().string();
					m_PathsMap[data["uuid"].as<uuids::uuid>()] = path;
					m_UUIDsMap[path] = data["uuid"].as<uuids::uuid>();
				}
			}
		}

		m_CurrentDirectory = m_RootDirectory;
	}

	void ResourceManager::CreateUUIDs()
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

	// If a file is no longer present, delete its meta file
	void ResourceManager::DeleteUUIDs()
	{
	}

	uuids::uuid ResourceManager::GetUUID(const std::filesystem::path& path)
	{
		return m_UUIDsMap[path];
	}

	Ref<Material> ResourceManager::LoadMaterial(const uuids::uuid& uuid)
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
				uniform.Data = MakeRef<glm::vec3>(data["Uniforms"][uniform.Name].as<glm::vec3>());
				break;
			case UniformType::Texture:
				uniform.Data = LoadTexture(data["Uniforms"][uniform.Name].as<uuids::uuid>());
				break;
			}
		}

		return material;
	}

	Ref<Shader> ResourceManager::LoadShader(const uuids::uuid& uuid)
	{
		return Shader::Create(m_PathsMap[uuid], uuid);
	}

	Ref<Texture> ResourceManager::LoadTexture(const uuids::uuid& uuid)
	{
		return Texture::Create(m_PathsMap[uuid], uuid);
	}

	//TODO - improve model and mesh api and useflow
	Ref<Mesh> ResourceManager::LoadModel(const uuids::uuid& uuid)
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

	Ref<LuaScript> ResourceManager::LoadScript(const uuids::uuid& uuid)
	{
		return MakeRef<LuaScript>(m_PathsMap[uuid], uuid);
	}
}