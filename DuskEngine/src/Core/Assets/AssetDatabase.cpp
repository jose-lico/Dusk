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

	}

	// At the moment there is no concept of project, so the engine simply loads all assets inside the "res" folder.
	void AssetDatabase::LoadProject()
	{
		//for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		//{
		//	// skip the import folder
		//	if (directoryEntry.path().filename() == ".import")
		//		continue;

		//	if(directoryEntry.is_directory())
		//	{
		//		m_CurrentDirectory = directoryEntry.path();
		//		LoadProject();
		//	}
		//	else
		//	{
		//		// If the file is a meta file, register it
		//		if (directoryEntry.path().extension() == ".meta")
		//		{
		//			//RegisterAsset(directoryEntry);
		//			continue;
		//		}
		//		// If the file does not have a meta file and is not a meta file itself
		//		else if (!std::filesystem::exists(directoryEntry.path().string() + ".meta") && directoryEntry.path().extension() != ".meta")
		//		{
		//			//CreateMetaFile(directoryEntry);
		//		}
		//	}
		//}

		//m_CurrentDirectory = m_RootDirectory;
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

		//AddToAssetDatabase(path, id);

		return id;
	}

	uuids::uuid AssetDatabase::GetUUID(const std::filesystem::path& path)
	{
		return m_UUIDsMap[path];
	}

	Material AssetDatabase::LoadMaterial(const uuids::uuid& uuid, AssetHandler* handler)
	{
		std::ifstream stream(m_PathsMap[uuid]);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		// dumb af
		auto shaderHandle = handler->AddToAssetPool<Shader>(data["Shader"].as<uuids::uuid>());

		Material material = Material(shaderHandle, handler, m_PathsMap[uuid], uuid);

		for (auto& uniform : material.m_Uniforms)
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

	Shader AssetDatabase::LoadShader(const uuids::uuid& uuid)
	{
		return CreateShader(m_PathsMap[uuid], uuid);
	}

	Texture AssetDatabase::LoadTexture(const uuids::uuid& uuid)
	{
		return CreateTexture(m_PathsMap[uuid], uuid);
	}

	//TODO - improve model and mesh api and useflow
	Mesh AssetDatabase::LoadModel(const uuids::uuid& uuid)
	{
		Model* m = new Model(m_PathsMap[uuid].string(), uuid);
		Mesh mesh = *m->GetFirstMesh();
		mesh.m_Type = MeshType::Model;
		mesh.m_UUID = uuid;
		mesh.m_Name = m->GetName();
		mesh.m_Path = m->GetPath();
		delete(m);
		return mesh;
	}

	UniqueRef<LuaScript> AssetDatabase::LoadScript(const uuids::uuid& uuid)
	{
		return MakeUnique<LuaScript>(m_PathsMap[uuid], uuid);
	}
}