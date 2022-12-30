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
		UUID = uuid;
		Path = path;
		Name = path.filename().string();
		m_ShaderHandle = shaderHandle;
		m_OwningHandler = owningHandler;
		m_Type = MaterialType::Custom;

		CreateUniforms();
		std::string message = "Created Material " + Name;
		LOG(message.c_str());
	}

	Material::Material(AssetHandler* owningHandler)
		:m_OwningHandler(owningHandler), m_ShaderHandle(0), m_Type(m_Type = MaterialType::Default)
	{
	}

	Material::Material(Material& material)
	{
		UUID = material.UUID;
		Path = material.Path;
		Name = material.Name;
		m_ShaderHandle = material.m_ShaderHandle;
		m_OwningHandler = material.m_OwningHandler;
		m_Type = material.m_Type;

		m_Uniforms = std::move(material.m_Uniforms);
		m_UniformsMap = std::move(material.m_UniformsMap);
	}

	Material::~Material()
	{
		std::string message = "Destroyed Material " + Name;
		LOG(message.c_str());
	}

	void Material::UploadUniforms(AssetHandler& assetHandler)
	{
		auto& shader = assetHandler.GetAsset<Shader>(m_ShaderHandle);
		
		OpenGLAPI::UseProgram(shader);

		unsigned int textSlot = 0;
		for (auto uniform : m_Uniforms)
		{
			switch(uniform.Type)
			{
			case UniformType::Vec3:
				OpenGLAPI::SetUniformVec3(shader, "u_" + uniform.Name, uniform.Data.vec3);
				break;
			case UniformType::Texture:
				OpenGLAPI::SetUniformInt(shader, "u_" + uniform.Name, textSlot);
				auto& texture = assetHandler.GetAsset<Texture>(uniform.Data.dataHandle);
				OpenGLAPI::BindTexture(texture, textSlot++);
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

		m_Uniforms.clear();
		m_UniformsMap.clear();
		CreateUniforms();
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
			m_UniformsMap[name]->Data.dataHandle = m_OwningHandler->AddToAssetPool<Texture>(texture.UUID);
		else
		{
			std::string message = "Texture uniform'" + name + "' doesnt exist";
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
		out << YAML::Key << "Material" << YAML::Value << Name;
		out << YAML::Key << "Shader" << YAML::Value << m_OwningHandler->GetAsset<Shader>(m_ShaderHandle);
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
				out << YAML::Key << uniform.Name << YAML::Value << m_OwningHandler->GetAsset<Texture>(uniform.Data.dataHandle);
				break;
			}
		}

		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	void Material::CreateUniforms()
	{
		auto& shader = m_OwningHandler->GetAsset<Shader>(m_ShaderHandle);

		for (auto& uniform : shader.UniformSpecs)
		{
			Uniform u;
			u.Name = uniform.Name;
			u.Type = uniform.Type;

			m_Uniforms.push_back(u);
		}

		UniformsDefaultValue();

		for (auto& uniform : m_Uniforms)
		{
			m_UniformsMap[uniform.Name] = &uniform;
		}
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

	Material Material::CreateDefaultMaterial(AssetHandler* owningHandler)
	{
		Material material(owningHandler);
		material.m_ShaderHandle = 0;
		material.CreateUniforms();
		material.Name = "Default Phong Material";
		return material;
	}
}