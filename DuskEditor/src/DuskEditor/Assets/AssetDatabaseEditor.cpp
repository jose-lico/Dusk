#include "AssetDatabaseEditor.h"

#include "DuskEditor/Importers/ImageImporter.h"

#include "Core/Application/Core.h"
#include "Core/Assets/AssetDatabase.h"
#include "Core/Assets/Asset.h"
#include "Utils/Serialization/Yaml.h"
#include "Utils/Profiling/Timer.h"

#include "uuid.h"

#include <string>
#include <fstream>


#undef CreateMetaFile

namespace DuskEngine
{
	AssetDatabaseEditor::AssetDatabaseEditor(AssetDatabase* database, const std::string& path)
		:m_EngineDB(database)
	{
		m_RootDirectory = path;
		m_CurrentDirectory = m_RootDirectory;
	}

	AssetDatabaseEditor::~AssetDatabaseEditor()
	{
		for (Asset* resource : m_ShaderDatabase)
			delete resource;

		for (Asset* resource : m_ModelDatabase)
			delete resource;

		for (Asset* resource : m_MaterialDatabase)
			delete resource;

		for (Asset* resource : m_ScriptsDatabase)
			delete resource;
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
				// If the file is a meta file, register it
				if (directoryEntry.path().extension() == ".meta")
				{
					RegisterAsset(directoryEntry);
					continue;
				}
				// If the file does not have a meta file and is not a meta file itself, create a meta file
				else if (!std::filesystem::exists(directoryEntry.path().string() + ".meta") && directoryEntry.path().extension() != ".meta")
				{
					CreateMetaFile(directoryEntry);
				}
			}
		}

		m_CurrentDirectory = m_RootDirectory;
	}

	void AssetDatabaseEditor::ImportAssets()
	{
		Timer importAssets("ImportAssets");
		for(auto& image : m_ImagesToImport)
		{
			ImportImage(std::get<0>(image), std::get<1>(image));
		}
	}

	void AssetDatabaseEditor::RegisterAsset(const std::filesystem::directory_entry& directoryEntry)
	{
		// If the file has been moved, removed or renamed, delete meta file.
		if (!std::filesystem::exists(m_CurrentDirectory / directoryEntry.path().stem()))
		{
			TRACE(directoryEntry.path().stem().string() + " has been moved, removed or renamed, deleting meta file.");
			std::filesystem::remove(directoryEntry);
			return;
		}

		std::ifstream stream(directoryEntry.path());

		YAML::Emitter out;
		YAML::Node data = YAML::Load(stream);

#ifdef DUSK_WINDOWS
		std::string path = m_CurrentDirectory.string() + "\\" + directoryEntry.path().stem().string();
#elif DUSK_LINUX
		std::string path = m_CurrentDirectory.string() + "/" + directoryEntry.path().stem().string();
#endif
		uuids::uuid uuid = data["uuid"].as<uuids::uuid>();

		m_EngineDB->m_PathsMap[uuid] = path;
		m_EngineDB->m_UUIDsMap[path] = uuid;

		AddToAssetDatabase(path, uuid);
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

		m_EngineDB->m_UUIDsMap[directoryEntry.path()] = id;
		m_EngineDB->m_PathsMap[id] = directoryEntry.path();

		AddToAssetDatabase(directoryEntry.path(), id);
	}

	void AssetDatabaseEditor::AddToAssetDatabase(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		Asset* resource = new Asset();
		resource->Path = path;
		resource->UUID = uuid;

		std::string extension = path.extension().string();

		bool wasAssigned = false;

		if (extension == ".png" || extension == ".jpg" || extension == ".jepg")
		{
			std::filesystem::path importFilePath = "res/.import/images/" + path.filename().string() + "-" + uuids::to_string(uuid) + ".import";
			if (!std::filesystem::exists(importFilePath))
			{
				m_ImagesToImport.push_back({path, importFilePath});
			}
		}
		else if (extension == ".glsl")
		{
			wasAssigned = true;
			m_ShaderDatabase.push_back(resource);
		}
		else if (extension == ".fbx" || extension == ".obj")
		{
			wasAssigned = true;
			m_ModelDatabase.push_back(resource);
		}
		else if (extension == ".material")
		{
			wasAssigned = true;
			m_MaterialDatabase.push_back(resource);
		}
		else if (extension == ".lua")
		{
			wasAssigned = true;
			m_ScriptsDatabase.push_back(resource);
		}

		if (!wasAssigned)
			delete resource;
	}
}