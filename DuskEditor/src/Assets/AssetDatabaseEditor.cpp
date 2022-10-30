#include "AssetDatabaseEditor.h"

#include "Core/Application/Core.h"
#include "Core/Assets/AssetDatabase.h"
#include "Utils/Serialization/Yaml.h"

#include "uuid.h"

#include <string>
#include <fstream>

#undef CreateMetaFile

namespace DuskEngine
{
	AssetDatabaseEditor::AssetDatabaseEditor(AssetDatabase* database)
		:m_EngineDB(database)
	{
		m_CurrentDirectory = m_RootDirectory;
	}

	AssetDatabaseEditor::~AssetDatabaseEditor()
	{
	}

	void AssetDatabaseEditor::RegisterAssets()
	{
		
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			// Skip the import folder
			if (directoryEntry.path().filename() == ".import")
				continue;
			if (directoryEntry.is_directory())
			{
				m_CurrentDirectory = directoryEntry.path();
				RegisterAssets();
			}
			else
			{
				// If the file does not have a meta file and is not a meta file itself, create a meta file
				if (!std::filesystem::exists(directoryEntry.path().string() + ".meta") && directoryEntry.path().extension() != ".meta")
				{
					CreateMetaFile(directoryEntry);
				}
			}
		}
	}

	void AssetDatabaseEditor::ImportAssets()
	{
	}

	void AssetDatabaseEditor::CreateMetaFile(const std::filesystem::directory_entry& directoryEntry)
	{
		LOG("Creating meta file for " + directoryEntry.path().filename().string());

		uuids::uuid const id = uuids::uuid_system_generator{}();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "uuid" << YAML::Value << id;

		std::string metaName = directoryEntry.path().string() + ".meta";
		std::ofstream fout(metaName.c_str());
		fout << out.c_str();
		fout.close();

		//m_EngineDB->m_UUIDsMap[directoryEntry.path()] = id;
		//m_EngineDB->m_PathsMap[id] = directoryEntry.path();

		//m_EngineDB->AddToAssetDatabase(directoryEntry.path(), id);
	}
}