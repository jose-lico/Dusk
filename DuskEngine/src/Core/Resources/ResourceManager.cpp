#include "pch.h"
#include "ResourceManager.h"

#include <iostream>
#include <iosfwd>
#include "Core/Renderer/Resources/Shader.h"

#include "Core/Macros/LOG.h"
#include "Utils/Serialization/Yaml.h"

#include <yaml-cpp/yaml.h>

namespace DuskEngine
{
	std::filesystem::path ResourceManager::m_CurrentDirectory = "res";

	std::unordered_map<std::string, std::string> ResourceManager::m_UUIDsMap;

	void ResourceManager::CreateUUIDs()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if(directoryEntry.is_directory())
			{
				std::cout << directoryEntry.path().string() << std::endl;
				m_CurrentDirectory = directoryEntry.path();
				CreateUUIDs();
			}
			else
			{
				std::filesystem::path metaFile = directoryEntry.path().string() + ".meta";
				// if meta file doesnt exist and file isnt already a meta file
				if(!std::filesystem::exists(metaFile) && directoryEntry.path().extension() != ".meta")
				{
					std::string message = "Meta file not present for " + directoryEntry.path().filename().string();
					std::cout << message << std::endl;
					std::ofstream myfile;
					std::string metaName = directoryEntry.path().string() + ".meta";

					myfile.open(metaName.c_str(), std::fstream::app);
					uuids::uuid const id = uuids::uuid_system_generator{}();
					myfile << id;
					myfile.close();
				}
			}
		}
		m_CurrentDirectory = "res";
	}

	void ResourceManager::LoadUUIDs()
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
				if(directoryEntry.path().extension() == ".meta")
				{
					std::ifstream infile(directoryEntry.path());

					if (infile.good())
					{
						std::string sLine;
						getline(infile, sLine);
						std::cout << sLine << std::endl;
						std::string path = m_CurrentDirectory.string() + "/" + directoryEntry.path().stem().string();
						std::cout << m_CurrentDirectory.string() + "/" + directoryEntry.path().stem().string() << std::endl;
						m_UUIDsMap[sLine] = path;
					}
				}
			}
		}
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

	// Separate this into more functions later
	Ref<Material> ResourceManager::LoadMaterial(const std::string& uuid)
	{
		std::ifstream stream(m_UUIDsMap[uuid]);
		
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		auto shader = Shader::Create(m_UUIDsMap[data["Shader"].as<std::string>()]);
		//shader->m_UUID = *uuids::uuid::from_string(m_UUIDsMap[data["Shader"].as<std::string>()]);

		Ref<Material> material = MakeRef<Material>(shader, data["Material"].as<std::string>());

		for (auto& uniform : material->m_Uniforms)
		{
			switch (uniform.Type)
			{
			case UniformType::Vec3:
				//uniform.Data = MakeRef<glm::vec3>(data[uniform.Name].as<glm::vec3>());
				uniform.Data = MakeRef<glm::vec3>(1.0f);
				break;
			case UniformType::Texture:
				auto texture = Texture::Create(m_UUIDsMap[data[uniform.Name].as<std::string>()]);
				//texture->m_UUID = *uuids::uuid::from_string(m_UUIDsMap[data[uniform.Name].as<std::string>()]);
				uniform.Data = texture;
				break;
			}
		}

		// Set uniforms
		return material;
	}
}