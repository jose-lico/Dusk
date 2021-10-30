#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Shader.h"

#include <vector>
#include <string>
#include <memory>

namespace DuskEngine
{
	struct Uniform
	{
		std::string Name;
		UniformType Type;
		void* Data = nullptr;

		Uniform(const std::string& name, UniformType type) : Name(name) , Type(type)
		{}
	};

	class DUSK_API Material
	{
	public:
		Material(std::shared_ptr<Shader>& shader); // or path
		~Material() {};
		void SetUniforms();
	private:
		std::vector<Uniform> m_Uniforms;
		std::shared_ptr<Shader> m_Shader;

		// friend class inspector
		friend class InspectorPanel;
	};
}