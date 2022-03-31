#include "pch.h"
#include "ResourceManager.h"

#include <iostream>
#include <iosfwd>

#include "Core/Resources/Resources/Shader.h"
#include "Core/Resources/Resources/Texture.h"

#include "Core/Macros/LOG.h"
#include "Utils/Serialization/Yaml.h"

namespace DuskEngine
{
	std::filesystem::path ResourceManager::m_RootDirectory = "res";
	std::filesystem::path ResourceManager::m_CurrentDirectory;

	std::unordered_map<uuids::uuid, std::filesystem::path> ResourceManager::m_UUIDsMap;

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
					std::ifstream infile(directoryEntry.path());

					// TODO -> This can be cleaned up a lot
					if (infile.good())
					{
						std::string sLine; 
						getline(infile, sLine);
						std::string path = m_CurrentDirectory.string() + "/" + directoryEntry.path().stem().string();
						m_UUIDsMap[*uuids::uuid::from_string(sLine)] = path;
					}
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

					std::ofstream myfile;
					std::string metaName = directoryEntry.path().string() + ".meta";

					myfile.open(metaName.c_str(), std::fstream::app);
					uuids::uuid const id = uuids::uuid_system_generator{}();
					myfile << id;
					myfile.close();

					m_UUIDsMap[id] = directoryEntry.path();
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
		std::ifstream infile(path);
		std::string sLine;
		if (infile.good())
		{
			getline(infile, sLine);
			return *uuids::uuid::from_string(sLine);
		}
	}

	Ref<Material> ResourceManager::LoadMaterial(const uuids::uuid& uuid)
	{
		std::ifstream stream(m_UUIDsMap[uuid]);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		auto shader = LoadShader(data["Shader"].as<uuids::uuid>());

		Ref<Material> material = MakeRef<Material>(shader, m_UUIDsMap[uuid], uuid);

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
		return Shader::Create(m_UUIDsMap[uuid], uuid);
	}

	Ref<Texture> ResourceManager::LoadTexture(const uuids::uuid& uuid)
	{
		return Texture::Create(m_UUIDsMap[uuid], uuid);
	}

	//TODO - improve model and mesh api and useflow
	Ref<Mesh> ResourceManager::LoadModel(const uuids::uuid& uuid)
	{
		Model* m = new Model(m_UUIDsMap[uuid].string());
		Ref<Mesh> mesh = m->GetFirstMesh();
		mesh->m_Type = MeshType::Model;
		mesh->m_UUID = uuid;
		mesh->m_Name = m->GetName();
		mesh->m_Path = m->GetPath();
		delete(m);
		return mesh;
	}
}