#include "pch.h"
#include "Material.h"

#include "Core/Macros/LOG.h"

namespace DuskEngine
{
	Material::Material(std::shared_ptr<Shader>& shader)
		:m_Shader(shader)
	{
		// setup shader program if needed

		// get uniform names and types from shader

		for(auto uniform : shader->UniformSpecs)
		{
			auto u = Uniform(uniform.Name, uniform.Type);
			u.Data = malloc(sizeof(glm::vec3)); // just for testing
			*(glm::vec3*)u.Data = { 1.0f, 0.0f, 0.0f };
			m_Uniforms.push_back(u);
		}
	}

	void Material::SetUniforms()
	{
		for (auto uniform : m_Uniforms)
		{
			// switch uniform type
			switch(uniform.Type)
			{
			case UniformType::Vec3:
				m_Shader->SetUniformVec3("u_" + uniform.Name, *(glm::vec3*)uniform.Data); // u_ + dodgy
				break;
			case UniformType::Texture: 
				break;
			}
		}
	}
}