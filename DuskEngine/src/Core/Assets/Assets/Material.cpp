#include "pch.h"
#include "Material.h"

#include "Texture.h"
#include "Shader.h"

#include "Core/Macros/LOG.h"
#include "Core/Assets/AssetHandler.h"
#include "Core/Assets/AssetManager.h"
#include "Utils/Serialization/Yaml.h"

namespace DuskEngine
{
	Material::Material(Ref<Shader>& shader, const std::filesystem::path& path, const uuids::uuid& uuid)
		:m_Shader(shader)
	{
		m_UUID = uuid;
		m_Path = path;
		m_Name = path.filename().string();

		CreateUniforms();
		LOG("Created Material " + m_Name)
	}

	/*Material::Material(Ref<Shader>& shader, const std::string& name)
		:m_Shader(shader)
	{
		if (name.empty())
			m_Name = m_Shader->GetName();
		else
			m_Name = name;

		CreateUniforms();
		LOG("Created Material " + m_Name)
	}*/

	/*Material::Material(const std::string& shaderPath, const std::string& name)
	{
		m_Shader = Shader::Create(shaderPath);

		if (name.empty())
			m_Name = m_Shader->GetName();
		else
			m_Name = name;

		CreateUniforms();
		LOG("Created Material " + m_Name)
	}*/

	Material::~Material()
	{
		LOG("Deleted Material " + m_Name)
	}

	void Material::UploadUniforms(AssetHandler& assetHandler)
	{
		m_Shader->Bind();
		unsigned int textSlot = 0;
		for (auto uniform : m_Uniforms)
		{
			// switch uniform type
			switch(uniform.Type)
			{
			case UniformType::Vec3:
				m_Shader->SetUniformVec3("u_" + uniform.Name, uniform.Data.vec3);
				break;
			case UniformType::Texture:
				m_Shader->SetUniformInt("u_" + uniform.Name, textSlot);
				auto& texture = assetHandler.TexturePool(uniform.Data.dataHandle);
				texture->Bind(textSlot++);
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
				uniform.Data.dataHandle = AssetManager::GetUUID("res/textures/white.png");				
				break;
			}
		}
	}

	void Material::SetShader(Ref<Shader>& shader)
	{
		m_Shader = shader;

		m_Uniforms.clear();
		m_UniformsMap.clear();
		CreateUniforms();
	}

	void Material::SetFloat(const std::string& name, float f)
	{
		if (m_UniformsMap.find(name) != m_UniformsMap.end())
			m_UniformsMap[name]->Data.fValue = f;
		else
			WARN("Uniform doesnt exist")
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

	void Material::SetTexture(const std::string& name, Ref<Texture>& texture)
	{
		if (m_UniformsMap.find(name) != m_UniformsMap.end())
			m_UniformsMap[name]->Data.dataHandle = texture->GetUUID();
		else
			WARN("Texture '" + name + "' doesnt exist")
	}

	const uuids::uuid& Material::GetShaderUUID()
	{
		return m_Shader->GetUUID();
	}

	void Material::SerializeText(const std::string& path, bool propagate)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Material" << YAML::Value << m_Name;
		out << YAML::Key << "Shader" << YAML::Value << m_Shader;
		out << YAML::Key << "Uniforms" << YAML::Value << m_Uniforms;

		std::ofstream fout(path);
		fout << out.c_str();

		if(propagate)
		{
			AssetHandler::PropagateMaterialChange(this);
		}
	}

	void Material::CreateUniforms()
	{
		for (auto uniform : m_Shader->UniformSpecs)
		{
			auto u = Uniform(uniform.Name, uniform.Type);
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
		out << YAML::Key << "Shader" << YAML::Value << AssetManager::GetUUID("res/shaders/simpleColor.glsl");
		out << YAML::Key << "Uniforms";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << glm::vec3(1.0);
		out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream material(path);
		material << out.c_str();
	}

}