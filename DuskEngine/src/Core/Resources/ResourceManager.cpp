#include "pch.h"
#include "ResourceManager.h"

#include <iostream>
#include <iosfwd>
#include "Core/Renderer/Resources/Shader.h"

#include "Core/Macros/LOG.h"
#include "Utils/Serialization/Yaml.h"

namespace DuskEngine
{
	std::filesystem::path ResourceManager::m_RootDirectory = "res";
	std::filesystem::path ResourceManager::m_CurrentDirectory;

	std::unordered_map<std::string, std::string> ResourceManager::m_UUIDsMap;

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

					if (infile.good())
					{
						std::string sLine;
						getline(infile, sLine);
						//std::cout << sLine << std::endl;
						std::string path = m_CurrentDirectory.string() + "/" + directoryEntry.path().stem().string();
						//std::cout << m_CurrentDirectory.string() + "/" + directoryEntry.path().stem().string() << std::endl;
						m_UUIDsMap[sLine] = path;
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

				// If meta file does not exist and file is not already a meta file
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

					m_UUIDsMap[uuids::to_string(id)] = directoryEntry.path().string();
				}
			}
		}

		m_CurrentDirectory = m_RootDirectory;
	}

	// If a file is no longer present, delete its meta file
	void ResourceManager::DeleteUUIDs()
	{
	}

	std::string ResourceManager::GetUUID(const std::string& path)
	{
		std::ifstream infile(path);
		std::string sLine;
		if (infile.good())
		{
			getline(infile, sLine);
			return sLine;
		}
	}

	Ref<Material> ResourceManager::LoadMaterial(const std::string& uuid)
	{
		std::ifstream stream(m_UUIDsMap[uuid]);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		auto shader = LoadShader(data["Shader"].as<std::string>());

		Ref<Material> material = MakeRef<Material>(shader, data["Material"].as<std::string>());
		material->m_UUID = *uuids::uuid::from_string(uuid);

		for (auto& uniform : material->m_Uniforms)
		{
			switch (uniform.Type)
			{
			case UniformType::Vec3:
				uniform.Data = MakeRef<glm::vec3>(data[uniform.Name].as<glm::vec3>());
				break;
			case UniformType::Texture:
				auto texture = Texture::Create(m_UUIDsMap[data[uniform.Name].as<std::string>()]);
				uniform.Data = texture;
				break;
			}
		}

		return material;
	}

	Ref<Shader> ResourceManager::LoadShader(const std::string& uuid)
	{
		auto shader = Shader::Create(m_UUIDsMap[uuid]);
		shader->m_UUID = *uuids::uuid::from_string(uuid);

		return shader;
	}

	Ref<Texture> ResourceManager::LoadTexture(const std::string& uuid)
	{
		auto texture = Texture::Create(m_UUIDsMap[uuid]);
		texture->m_UUID = *uuids::uuid::from_string(uuid);

		return texture;
	}

	Ref<Model> ResourceManager::LoadModel(const std::string& uuid)
	{
		return Ref<Model>();
	}
}