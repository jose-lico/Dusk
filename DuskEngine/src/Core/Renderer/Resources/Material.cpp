#include "pch.h"
#include "Material.h"

#include "Core/Macros/LOG.h"

namespace DuskEngine
{
	Material::Material(std::shared_ptr<Shader>& shader, const std::string& name)
		:m_Shader(shader)
	{
		if (name.empty())
			m_Name = m_Shader->GetName();
		else
			m_Name = name;

		CreateUniforms();
	}

	Material::Material(const std::string& shaderPath, const std::string& name)
	{
		m_Shader = Shader::Create(shaderPath);

		if (name.empty())
			m_Name = m_Shader->GetName();
		else
			m_Name = name;

		CreateUniforms();
	}

	Material::~Material()
	{
	}

	void Material::UploadUniforms()
	{
		m_Shader->Bind();
		unsigned int textSlot = 0;
		for (auto uniform : m_Uniforms)
		{
			// switch uniform type
			switch(uniform.Type)
			{
			case UniformType::Vec3:
				m_Shader->SetUniformVec3("u_" + uniform.Name, *std::static_pointer_cast<glm::vec3>(uniform.Data)); // u_ + dodgy
				break;
			case UniformType::Texture:
				m_Shader->SetUniformInt("u_" + uniform.Name, textSlot);
				std::static_pointer_cast<Texture>(uniform.Data)->Bind(textSlot++);
				break;
			}
		}
	}

	void Material::SetFloat(const std::string& name, float f)
	{
		if (m_UniformsMap.find(name) != m_UniformsMap.end())
			m_UniformsMap[name]->Data = std::make_shared<float>(f);
		else
			LOG("Uniform doesnt exist")
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
			m_UniformsMap[name]->Data = texture;
		else
			LOG("Texture '" + name + "' doesnt exist")
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
	}
}