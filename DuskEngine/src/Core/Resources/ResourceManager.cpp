#include "pch.h"
#include "ResourceManager.h"

#include <iostream>
#include <iosfwd>

namespace DuskEngine
{
	ResourceManager::ResourceManager()
	{
		m_CurrentDirectory = "res/Root";
	}

	ResourceManager::~ResourceManager()
	{
	}

	void ResourceManager::LoadUUIDs()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if(directoryEntry.is_directory())
			{
				std::cout << directoryEntry.path().string() << std::endl;
				m_CurrentDirectory = directoryEntry.path();
				LoadUUIDs();
			}
			else
			{
				std::filesystem::path hey = "res/Root/" + directoryEntry.path().filename().string() + ".meta";
				if(!std::filesystem::exists(hey) && directoryEntry.path().extension() != ".meta")
				{
					std::string message = "Meta file not present for " + directoryEntry.path().filename().string();
					std::cout << message << std::endl;
					std::ofstream myfile;
					std::string metaName = directoryEntry.path().string() + ".meta";

					myfile.open(metaName.c_str(), std::fstream::app);
					uuids::uuid const id = uuids::uuid_system_generator{}();
					myfile << "This is a meta file\n";
					myfile << id;
					myfile.close();
				}
			}
		}
	}

	void ResourceManager::LoadResources(Ref<Scene>& scene)
	{
	}
}