#include "pch.h"
#include "SceneSerializer.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/Components.h"
#include "Core/Assets/AssetDatabase.h"
#include "Core/ECS/ComponentRegistry.cpp"
#include "Core/Assets/AssetHandler.h"
#include "Utils/Rendering/PrimitiveMesh.h"
#include "Utils/Serialization/Yaml.h"
#include "Utils/Profiling/Timer.h"

#include "glm/glm.hpp"

namespace DuskEngine
{
	template<typename T>
	static void SerializeComponentText(const std::string& name, YAML::Emitter& out, Entity entity);

	AssetHandler* SceneSerializer::m_AssetHandler = nullptr;

    void SceneSerializer::SerializeText(const Ref<Scene>& scene, const std::string& path)
    {
		//Timer timer("Serialize Text");

		m_AssetHandler = scene->m_AssetHandler;

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, scene.get() };

				out << YAML::BeginMap;
				out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<Meta>().entityHandle;

				SerializeComponentText<Meta>("Meta", out, entity);
				SerializeComponentText<Transform>("Transform", out, entity);
				SerializeComponentText<Camera>("Camera", out, entity);
				SerializeComponentText<MeshRenderer>("MeshRenderer", out, entity);
				SerializeComponentText<Light>("Light", out, entity);
				SerializeComponentText<Script>("Script", out, entity);

				out << YAML::EndMap;

			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
    }

    void SceneSerializer::SerializeBinary(const Ref<Scene>& scene, const std::string& path)
    {
    }

    bool SceneSerializer::DeserializeText(const Ref<Scene>& scene, const std::string& path)
    {
		Timer timer("Deserialize Text");

		std::ifstream stream(path);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				std::string name;
				auto meta = entity["Meta"];
				if (meta)
					name = meta["name"].as<std::string>();

				Entity deserializedEntity = scene->CreateEntity(name);

				deserializedEntity.GetComponent<Meta>().enabled = meta["enabled"].as<bool>();
				deserializedEntity.GetComponent<Meta>().entityHandle = entity["Entity"].as<uuids::uuid>();

				auto transform = entity["Transform"];
				if (transform)
				{
					auto& t = deserializedEntity.GetComponent<Transform>();
					t.position = transform["position"].as<glm::vec3>();
					t.rotation = transform["rotation"].as<glm::vec3>();
					t.scale = transform["scale"].as<glm::vec3>();
				}

				auto camera = entity["Camera"];
				if (camera)
				{
					auto& c = deserializedEntity.AddComponent<Camera>();
					c.projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f); // TODO should be a serialized property
					c.main = camera["main"].as<bool>();
				}

				auto meshRenderer = entity["MeshRenderer"];
				if(meshRenderer)
				{
					auto& mr = deserializedEntity.AddComponent<MeshRenderer>();

					auto material = meshRenderer["material"];
					int type = material["fileID"].as<int>();
					if (type == 0)
						mr.materialHandle = 0;
					else
						mr.materialHandle = scene->m_AssetHandler->AddToAssetPool<Material>(material["uuid"].as<uuids::uuid>());

					auto mesh = meshRenderer["mesh"];

					type = mesh["fileID"].as<int>();

					if (type == 0)
						mr.meshHandle = 0;
					if (type == 1)
						mr.meshHandle = 1;
					if (type == 2)
						mr.meshHandle = scene->m_AssetHandler->AddToAssetPool<Mesh>(mesh["uuid"].as<uuids::uuid>());;
				}

				auto light = entity["Light"];
				if (light)
				{
					auto& l = deserializedEntity.AddComponent<Light>();

					l.color = light["color"].as<glm::vec3>();
					l.type = Light::LightTypeEnum(light["type"].as<std::string>());
				}

				auto scriptData = entity["Script"];
				if(scriptData)
				{
					auto& s = deserializedEntity.AddComponent<Script>();
					auto script = scriptData["script"].as<uuids::uuid>();
					s.luaScriptHandle = scene->m_AssetHandler->AddToLuaScriptPool(entity["Entity"].as<uuids::uuid>(), script);
					scene->m_AssetHandler->LuaScriptPool(s.luaScriptHandle)->m_Ent = deserializedEntity;
				}
			}
		}

        return true;
    }

    bool SceneSerializer::DeserializeBinary(const Ref<Scene>& scene, const std::string& path)
    {
        return false;
    }

	template<typename T>
	void SerializeComponentText(const std::string& name, YAML::Emitter& out, Entity entity)
	{
		if(entity.HasComponent<T>())
		{
			out << YAML::Key << name.c_str();
			out << YAML::BeginMap;

			auto& component = entity.GetComponent<T>();

			rttr::type type = rttr::type::get(component);

			for (auto& prop : type.get_properties())
			{
				out << YAML::Key << prop.get_name().to_string() << YAML::Value << prop.get_value(component);
			}

			out << YAML::EndMap;
		}
	}
}