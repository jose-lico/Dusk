#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Resources/Resource.h"
#include "Utils/Memory/Memory.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

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

	class DUSK_EXPORT Shader : public Resource
	{
	public:
		static Ref<Shader> Create(const std::string& filepath, const std::string& name = "");

		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(const std::string& name, int i) = 0;
		virtual void SetUniformFloat(const std::string& name, float f) = 0;
		virtual void SetUniformVec2(const std::string& name, const glm::vec2& v) = 0;
		virtual void SetUniformVec3(const std::string& name, const glm::vec3& v) = 0;
		virtual void SetUniformVec4(const std::string& name, const glm::vec4& v) = 0;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& m) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& m) = 0;

		virtual const std::string& GetName() const = 0;

		std::vector<UniformSpec> UniformSpecs;

		friend class ResourceManager;
	};
}