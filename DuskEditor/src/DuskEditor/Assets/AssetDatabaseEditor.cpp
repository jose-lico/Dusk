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
			// Skip these folders
			if (directoryEntry.path().filename() == ".import" || directoryEntry.path().filename() == ".editor" || directoryEntry.path().filename() == "logs")
				continue;
			// skip runtime stuff
			if (directoryEntry.path().extension() == ".exe" || directoryEntry.path().extension() == ".pdb")
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
		m_ImagesToImport.resize(0);
	}

	void AssetDatabaseEditor::CopyAsset(const std::filesystem::path& assetPath, const std::filesystem::path& currentDir)
	{
		std::filesystem::path targetPath = currentDir / assetPath.filename();

		if (std::filesystem::exists(targetPath))
		{
			TRACE("Swapping asset " + assetPath.filename().string());
			std::filesystem::remove_all(targetPath);
			std::filesystem::copy(assetPath, targetPath, std::filesystem::copy_options::recursive);
		}
		else
		{
			TRACE("Copying asset " + assetPath.filename().string());
			std::filesystem::copy(assetPath, targetPath, std::filesystem::copy_options::recursive);
		}

		if (!std::filesystem::is_directory(targetPath))
		{
			std::filesystem::remove(targetPath.string() + ".meta"); // Create new meta file for both copy and swap. Perhaps not necessary for swap
			CreateMetaFile(targetPath);
		}
		else
		{
			m_CurrentDirectory = targetPath;
			RegisterAssets();
		}
	}

	void AssetDatabaseEditor::RegisterAsset(const std::filesystem::path& assetPath)
	{
		// If the file has been moved, removed or renamed, delete meta file.
		if (!std::filesystem::exists(m_CurrentDirectory / assetPath.stem()))
		{
			TRACE(assetPath.stem().string() + " has been moved, removed or renamed, deleting meta file.");
			std::filesystem::remove(assetPath);
			return;
		}

		std::ifstream stream(assetPath);

		YAML::Emitter out;
		YAML::Node data = YAML::Load(stream);

		std::filesystem::path path = m_CurrentDirectory / assetPath.stem();

		uuids::uuid uuid = data["uuid"].as<uuids::uuid>();

		m_EngineDB->m_PathsMap[uuid] = path;
		m_EngineDB->m_UUIDsMap[path] = uuid;

		AddToAssetDatabase(path, uuid);
	}

	void AssetDatabaseEditor::CreateMetaFile(const std::filesystem::path& assetPath)
	{
		LOG("Creating meta file for " + assetPath.filename().string());

		uuids::uuid const id = uuids::uuid_system_generator{}();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "uuid" << YAML::Value << id;

		std::string metaName = assetPath.string() + ".meta";
		std::ofstream fout(metaName.c_str());
		fout << out.c_str();
		fout.close();

		m_EngineDB->m_UUIDsMap[assetPath] = id;
		m_EngineDB->m_PathsMap[id] = assetPath;

		AddToAssetDatabase(assetPath, id);
	}

	void AssetDatabaseEditor::AddToAssetDatabase(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		Asset* resource = new Asset();
		resource->Path = path;
		resource->Name = path.filename().stem().string();
		resource->UUID = uuid;

		std::string extension = path.extension().string();

		bool wasAssigned = false;

		if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
		{
			std::filesystem::path importFilePath = m_RootDirectory.string() + "/.import/images/" + path.filename().string() + "-" + uuids::to_string(uuid) + ".import";
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
		else if (extension == ".glb")
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