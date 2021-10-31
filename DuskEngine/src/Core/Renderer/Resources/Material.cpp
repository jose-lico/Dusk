#include "pch.h"
#include "Material.h"

#include "Core/Macros/LOG.h"

// This is terrible
#include "Platform/OpenGL/GLCommon.h"

namespace DuskEngine
{
	Material::Material(std::shared_ptr<Shader>& shader)
		:m_Shader(shader)
	{
		for(auto uniform : shader->UniformSpecs)
		{
			auto u = Uniform(uniform.Name, uniform.Type);
			u.Data = malloc(GetSize(u.Type));
			m_Uniforms.push_back(u);
		}

		for (auto& uniform : m_Uniforms)
		{
			m_UniformsMap[uniform.Name] = &uniform;
		}
	}

	Material::~Material()
	{
		for (auto uniform : m_Uniforms)
		{
			free(uniform.Data);
		}
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
				m_Shader->SetUniformVec3("u_" + uniform.Name, *(glm::vec3*)uniform.Data); // u_ + dodgy
				break;
			case UniformType::Texture: 
				m_Shader->SetUniformInt("u_" + uniform.Name, textSlot);
				glBindTextureUnit(textSlot++, *(unsigned int*)uniform.Data); // forgive me
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