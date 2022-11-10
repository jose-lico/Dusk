#pragma once

#include "Core/Assets/Asset.h"

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

	struct Shader : public Asset
	{
		uint32_t ResourceID;
		std::vector<UniformSpec> UniformSpecs;
		std::unordered_map<std::string, int> UniformLocations;
	};

	Shader CreateShader(const std::filesystem::path& path, const uuids::uuid& uuid);
	Shader CreateShader(const char* vertSource, const char* fragSource);
}