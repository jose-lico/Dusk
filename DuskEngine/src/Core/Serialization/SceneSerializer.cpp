#include "pch.h"
#include "SceneSerializer.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/Components.h"
#include "Core/Macros/LOG.h"

#include "Core/Resources/ResourceManager.h"

#include "Utils/Rendering/PrimitiveMesh.h"
#include "Utils/Serialization/Yaml.h"

#include <glm/glm.hpp>

namespace DuskEngine
{
	template<typename T>
	static void SerializeComponentText(const std::string& name, YAML::Emitter& out, Entity entity);	
	
	template<typename T>
	static void DeserializeComponentText(const std::string& name, YAML::Node& data, Entity entity);

    void SceneSerializer::SerializeText(const Ref<Scene>& scene, const std::string& path)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        scene->m_Registry.each([&](auto entityID)
            {
                Entity entity = { entityID, scene.get() };
                
				out << YAML::BeginMap; // Entity
				out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

				SerializeComponentText<Meta>("Meta", out, entity);
				SerializeComponentText<Transform>("Transform", out, entity);
				SerializeComponentText<Camera>("Camera", out, entity);
				SerializeComponentText<MeshRenderer>("MeshRenderer", out, entity);
				SerializeComponentText<Light>("Light", out, entity);

				out << YAML::EndMap; // Entity
				
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

				auto transform = entity["Transform"];
				if (transform)
				{
					// Entities always have transforms
					auto& t = deserializedEntity.GetComponent<Transform>();
					t.position = transform["position"].as<glm::vec3>();
					t.rotation = transform["rotation"].as<glm::vec3>();
					t.scale = transform["scale"].as<glm::vec3>();
				}

				auto camera = entity["Camera"];
				if (camera)
				{
					// Entities always have transforms
					auto& c = deserializedEntity.AddComponent<Camera>();
					c.main = camera["main"].as<bool>();
				}

				auto meshRenderer = entity["MeshRenderer"];
				if(meshRenderer)
				{
					auto& mr = deserializedEntity.AddComponent<MeshRenderer>();

					mr.material = ResourceManager::LoadMaterial(meshRenderer["material"].as<std::string>());
					mr.mesh = PrimitiveMesh::Cube(); // replace with actual model, from file or PrimitiveMesh
				}

				auto light = entity["Light"];
				if (light)
				{
					auto& l = deserializedEntity.AddComponent<Light>();

					l.color = light["color"].as<glm::vec3>();
					l.type = Light::LightTypeEnum(light["type"].as<std::string>());
				}
			}
		}

        return false;
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

			visit_struct::for_each(component, [&out](const char* name, const auto& value)
				{
					out << YAML::Key << name << YAML::Value << value;
				});

			out << YAML::EndMap;
		}
	}

	template<typename T>
	void DeserializeComponentText(const std::string& name, YAML::Node& data, Entity entity)
	{
		/*auto componentData = data[name];
		if(componentData)
		{
			auto& component = entity.AddComponent<T>();
			int index = 0;
			visit_struct::for_each(component, [&componentData, &component, &index](const char* name, auto& value)
				{
					if (std::is_same<visit_struct::type_at<index, T>, bool>())
					{
						int i = 5;
					}
					index++;
				});	
		}*/
	}
}