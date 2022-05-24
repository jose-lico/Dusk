#pragma once

#include "Core/Assets/Assets/Material.h"
#include "Core/Assets/Assets/Shader.h"
#include "Core/Assets/Assets/Texture.h"
#include "Core/Assets/Assets/Mesh.h"
#include "Core/ECS/Components/Light.h"
#include "Core/Scripting/LuaScript.h"

#include "yaml-cpp/yaml.h"
#include "glm/glm.hpp"
#include "rttr/variant.h"

namespace DuskEngine
{
	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const uuids::uuid& uuid)
	{
		out << YAML::Value << uuids::to_string(uuid);

		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<Mesh>& mesh)
	{
		out << YAML::BeginMap;

		auto type = mesh->GetType();

		switch (type)
		{
		case DuskEngine::MeshType::Quad:
			out << YAML::Key << "fileID" << YAML::Value << 0;
			break;
		case DuskEngine::MeshType::Cube:
			out << YAML::Key << "fileID" << YAML::Value << 1;
			break;
		case DuskEngine::MeshType::Model:
			out << YAML::Key << "fileID" << YAML::Value << 2;
			break;
		default:
			out << YAML::Key << "fileID" << YAML::Value << -1;
			break;
		}

		out << YAML::Key << "uuid" << YAML::Value << mesh->GetUUID();
		out << YAML::EndMap;

		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<Material>& material)
	{
		out << YAML::Value << material->GetUUID();

		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<Shader>& shader)
	{
		out << YAML::Value << shader->GetUUID();

		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const std::vector<Uniform>& uniforms)
	{
		out << YAML::BeginMap;

		for (auto& uniform : uniforms)
		{
			switch (uniform.Type)
			{
			case UniformType::Vec3:
				out << YAML::Key << uniform.Name << YAML::Value << uniform.Data.vec3;
				break;
			case UniformType::Texture:
				out << YAML::Key << uniform.Name << YAML::Value << uniform.Data.dataHandle;
				break;
			}
		}

		out << YAML::EndMap;

		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const LightType type)
	{
		out << YAML::Value << Light::LightTypeString(type);

		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const std::vector<Ref<LuaScript>>& scripts)
	{
		out << YAML::BeginMap;

		for (auto script : scripts)
		{
			out << YAML::Key << script->GetName() << YAML::Value << script->GetUUID();
		}

		out << YAML::EndMap;
		return out;
	}

	inline
	YAML::Emitter& operator<<(YAML::Emitter& out, const rttr::variant& var)
	{
		if(var.can_convert<glm::vec3>())
		{
			return out << YAML::Value << var.convert<glm::vec3>();
		}
		else if (var.can_convert<glm::vec4>())
		{
			return out << YAML::Value << var.convert<glm::vec4>();
		}
		else if(var.can_convert<std::string>())
		{
			return out << YAML::Value << var.convert<std::string>();
		}
		else if (var.can_convert<LightType>()) // enums need to come before bool
		{
			return out << YAML::Value << var.convert<LightType>();
		}
		else if (var.can_convert<bool>())
		{
			return out << YAML::Value << var.convert<bool>();
		}
		else if (var.can_convert<uuids::uuid>())
		{
			return out << YAML::Value << var.convert<uuids::uuid>();
		}
		else if (var.can_convert<Ref<Mesh>>())
		{
			return out << YAML::Value << var.convert<Ref<Mesh>>();
		}
		else if (var.can_convert<Ref<Material>>())
		{
			return out << YAML::Value << var.convert<Ref<Material>>();
		}
		else if (var.can_convert<Ref<Shader>>())
		{
			return out << YAML::Value << var.convert<Ref<Shader>>();
		}
		else if (var.can_convert<std::vector<Uniform>>())
		{
			return out << YAML::Value << var.convert<std::vector<Uniform>>();
		}
		else if (var.can_convert<std::vector<Ref<LuaScript>>>())
		{
			return out << YAML::Value << var.convert<std::vector<Ref<LuaScript>>>();
		}

		return out << YAML::Value << "Unrecognized type";
	}
}

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

	template<>
	struct convert<uuids::uuid>
	{
		static Node encode(const uuids::uuid& rhs)
		{
			Node node;
			node.push_back(uuids::to_string(rhs));
			node.SetStyle(EmitterStyle::Default);
			return node;
		}

		static bool decode(const Node& node, uuids::uuid& rhs)
		{
			rhs = *uuids::uuid::from_string(node.as<std::string>());
			return true;
		}
	};
}