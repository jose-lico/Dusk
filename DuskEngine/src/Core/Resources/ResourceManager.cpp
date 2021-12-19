#include "pch.h"
#include "ResourceManager.h"

#include <iostream>
#include <iosfwd>
#include "Core/Renderer/Resources/Shader.h"

namespace DuskEngine
{
	ResourceManager::ResourceManager()
	{
		m_CurrentDirectory = "res";
	}

	ResourceManager::~ResourceManager()
	{
	}

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

		if(m_UUIDsMap.find("278cbf4a-f5d1-4136-a5c6-ea242a6b17e1") != m_UUIDsMap.end())
		{
			std::cout << m_UUIDsMap["278cbf4a-f5d1-4136-a5c6-ea242a6b17e1"] << std::endl;
			Ref<Shader> shader = Shader::Create(m_UUIDsMap["278cbf4a-f5d1-4136-a5c6-ea242a6b17e1"]);
		}

	}

	void ResourceManager::LoadResources(Ref<Scene>& scene)
	{
	}
}