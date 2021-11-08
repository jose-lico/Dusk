#pragma once
#include "Core/Macros/DUSK_API.h"
#include "Core/Macros/LOG.h"

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
		std::shared_ptr<void> Data = nullptr;

		Uniform(const std::string& name, UniformType type) : Name(name) , Type(type)
		{}
	};

	class DUSK_EXPORT Material
	{
	public:
		Material(std::shared_ptr<Shader>& shader); // or path
		~Material();
		void SetUniforms();

		template<typename T>
		void SetUniformData(const std::string& name, T& data)
		{
			if (m_UniformsMap.find(name) != m_UniformsMap.end())
				m_UniformsMap[name]->Data = std::make_shared<T>(data);
			else
				APP_LOG("Uniform doesnt exist")
		}
	private:
		// Map is for direct access to set uniform values
		std::unordered_map<std::string, Uniform*> m_UniformsMap;

		// Vector is where the actual uniform values are located, for iteration - editor drawing and sending to shader
		std::vector<Uniform> m_Uniforms;
		std::shared_ptr<Shader> m_Shader;

		size_t GetSize(UniformType type);

		friend class InspectorPanel;
		friend class Scene;
	};
}