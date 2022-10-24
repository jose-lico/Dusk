#pragma once
#include "Core/Assets/Asset.h"

#include "glm/glm.hpp"

#include <unordered_map>

namespace DuskEngine 
{
	enum class UniformType
	{
		Vec3, Texture, Float, Int, Unknown
	};

	struct UniformSpec
	{
		std::string Name;
		UniformType Type;

		UniformSpec(const std::string& name, UniformType type) : Name(name), Type(type)
		{};
	};

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader : public Asset
	{
	public:
		Shader(const std::string& filepath, const std::string& name = "");
		Shader(const std::filesystem::path& path, const uuids::uuid& uuid);

		Shader(const Shader&) = delete;
		Shader& operator = (const Shader&) = delete;
		
		Shader(Shader&&) = default;
		Shader& operator = (Shader&&) = default;
		
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformInt(const std::string& name, int i);
		void SetUniformFloat(const std::string& name, float f);
		void SetUniformVec2(const std::string& name , const glm::vec2& v);
		void SetUniformVec3(const std::string& name , const glm::vec3& v);
		void SetUniformVec4(const std::string& name , const glm::vec4& v);
		void SetUniformMat3(const std::string& name , const glm::mat3& m);
		void SetUniformMat4(const std::string& name , const glm::mat4& m);

		std::vector<UniformSpec> UniformSpecs;
	private:
		ShaderProgramSource ParseShader();
		UniformType GetType(const std::string& type);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);

		std::unordered_map<std::string, int> m_uniformLocations;
		unsigned int m_ID;
	};
}