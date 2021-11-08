#include "pch.h"
#include "Material.h"

#include "Texture.h"

#include "Core/Macros/LOG.h"

namespace DuskEngine
{
	Material::Material(std::shared_ptr<Shader>& shader)
		:m_Shader(shader)
	{
		for(auto uniform : shader->UniformSpecs)
		{
			// maybe initiliaze uniforms to default value;
			auto u = Uniform(uniform.Name, uniform.Type);
			m_Uniforms.push_back(u);
		}

		for (auto& uniform : m_Uniforms)
		{
			m_UniformsMap[uniform.Name] = &uniform;
		}
	}

	
	Material::~Material()
	{
	}

	void Material::SetUniforms()
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
				std::static_pointer_cast<std::shared_ptr<Texture>>(uniform.Data)->get()->Bind(textSlot++);
				break;
			}
		}
	}

	size_t Material::GetSize(UniformType type)
	{
		{
			switch (type)
			{
			case UniformType::Vec3:		return sizeof(glm::vec3);
			case UniformType::Texture:  return sizeof(unsigned int);
			case UniformType::Float:	return sizeof(float);
			case UniformType::Int:		return sizeof(int);
			case UniformType::Unknown:	return 0;
			}
		}
	}
}