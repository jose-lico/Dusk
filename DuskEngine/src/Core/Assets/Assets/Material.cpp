#include "pch.h"
#include "Material.h"

#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/Texture.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Core/Application/Application.h"
#include "Core/Assets/AssetHandler.h"
#include "Core/Assets/AssetDatabase.h"
#include "Utils/Serialization/Yaml.h"

namespace DuskEngine
{
	Material::Material(Handle<Shader> shaderHandle, AssetHandler* owningHandler, const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		m_UUID = uuid;
		m_Path = path;
		m_Name = path.filename().string();
		m_ShaderHandle = shaderHandle;
		m_OwningHandler = owningHandler;

		CreateUniforms();
		std::string message = "Created Material " + m_Name;
		LOG(message.c_str());
	}

	Material::~Material()
	{
		std::string message = "Destroyed Material " + m_Name;
		LOG(message.c_str());
	}

	void Material::UploadUniforms(AssetHandler& assetHandler)
	{
		auto& shader = m_OwningHandler->AssetPool<Shader>(m_ShaderHandle);
		
		OpenGLAPI::UseProgram(shader.ResourceID);

		unsigned int textSlot = 0;
		for (auto uniform : m_Uniforms)
		{
			switch(uniform.Type)
			{
			case UniformType::Vec3:
				OpenGLAPI::SetUniformVec3(shader, "u_" + uniform.Name, uniform.Data.vec3);
				//shader.SetUniformVec3("u_" + uniform.Name, uniform.Data.vec3);
				break;
			case UniformType::Texture:
				OpenGLAPI::SetUniformInt(shader, "u_" + uniform.Name, textSlot);
				auto& texture = assetHandler.AssetPool<Texture>(uniform.Data.dataHandle);
				OpenGLAPI::BindTexture(textSlot++, texture.ResourceID);
				break;
			}
		}
	}

	void Material::UniformsDefaultValue()
	{
		for (auto& uniform : m_Uniforms)
		{
			switch (uniform.Type)
			{
			case UniformType::Vec3:
				
				uniform.Data.vec3 = glm::vec3(1.0f);
				
				break;
			case UniformType::Texture:
				uniform.Data.dataHandle = 0;
				break;
			}
		}
	}

	void Material::SetShader(const Handle<Shader> shader)
	{
		m_ShaderHandle = shader;
	}

	void Material::SetFloat(const std::string& name, float f)
	{
		if (m_UniformsMap.find(name) != m_UniformsMap.end())
			m_UniformsMap[name]->Data.fValue = f;
		else
			WARN("Uniform doesnt exist");
	}

	void Material::SetVec2(const std::string& name, glm::vec2& v)
	{
	}

	void Material::SetVec3(const std::string& name, glm::vec3& v)
	{
	}

	void Material::SetVec4(const std::string& name, glm::vec4& v)
	{
	}

	void Material::SetTexture(const std::string& name, Texture& texture)
	{
		if (m_UniformsMap.find(name) != m_UniformsMap.end())
			m_UniformsMap[name]->Data.dataHandle = m_OwningHandler->AddToAssetPool<Texture>(texture.GetUUID());
		else
		{
			std::string message = "Texture '" + name + "' doesnt exist";
			WARN(message.c_str());
		}
	}

	/*const uuids::uuid& Material::GetShaderUUID()
	{
		return m_Shader->GetUUID();
	}*/

	void Material::SerializeText(const std::string& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Material" << YAML::Value << m_Name;
		//out << YAML::Key << "Shader" << YAML::Value << m_OwningHandler->AssetPool<Shader>(m_ShaderHandle);
		//out << YAML::Key << "Uniforms" << YAML::Value << m_Uniforms;

		out << YAML::Key << "Uniforms";
		out << YAML::BeginMap;

		for (auto& uniform : m_Uniforms)
		{
			switch (uniform.Type)
			{
			case UniformType::Vec3:
				out << YAML::Key << uniform.Name << YAML::Value << uniform.Data.vec3;
				break;
			case UniformType::Texture:
				out << YAML::Key << uniform.Name << YAML::Value << m_OwningHandler->AssetPool<Texture>(uniform.Data.dataHandle);
				break;
			}
		}

		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	void Material::CreateUniforms()
	{
		auto& shader = m_OwningHandler->AssetPool<Shader>(m_ShaderHandle);

		for (auto uniform : shader.UniformSpecs)
		{
			Uniform u;
			u.Name = uniform.Name;
			u.Type = uniform.Type;

			m_Uniforms.push_back(u);
		}

		for (auto& uniform : m_Uniforms)
		{
			m_UniformsMap[uniform.Name] = &uniform;
		}

		UniformsDefaultValue();
	}

	void Material::CreateDefaultMaterial(const std::filesystem::path& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Material" << YAML::Value << "newMaterial";
		out << YAML::Key << "Shader" << YAML::Value << Application::Get().GetAssetDatabase().GetUUID("res/shaders/simpleColor.glsl");
		out << YAML::Key << "Uniforms";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << glm::vec3(1.0);
		out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream material(path);
		material << out.c_str();
	}
}