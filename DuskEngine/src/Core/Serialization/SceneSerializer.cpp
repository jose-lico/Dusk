#include "pch.h"
#include "SceneSerializer.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/Components.h"

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace DuskEngine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	template<typename T>
	static void SerializeComponentText(const std::string& name, YAML::Emitter& out, Entity entity);

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

			auto& c = entity.GetComponent<T>();
			visit_struct::for_each(c, [&out](const char* name, const auto& value)
				{
					out << YAML::Key << name << YAML::Value << value;
				});

			out << YAML::EndMap;
		}
	}
}