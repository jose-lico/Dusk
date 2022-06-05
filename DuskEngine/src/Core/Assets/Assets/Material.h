#pragma once
#include "Core/Application/Core.h"

#include "Core/Assets/Asset.h"
#include "Utils/Memory/Memory.h"
#include "Core/Assets/Handle.h"

#include "glm/glm.hpp"

#include <vector>
#include <string>
#include <memory>
#include <any>

namespace DuskEngine
{
	class Shader;
	class Texture;
	class AssetHandler;
	enum class UniformType;

	union UniformData
	{
		float fValue;
		uint32_t dataHandle;
		glm::vec2 vec2;
		glm::vec3 vec3;
		glm::vec4 vec4;

		UniformData() {};
	};

	struct Uniform
	{
		std::string Name;
		UniformType Type;
		
		UniformData Data;

		Uniform() = default;
		Uniform(const std::string& name, UniformType type) : Name(name), Type(type) {};

		/*Uniform(const std::string& name, UniformType type, float f) : Name(name), Type(type), fValue(f) {}
		Uniform(const std::string& name, UniformType type, glm::vec2& v) : Name(name), Type(type), vec2(v) {}
		Uniform(const std::string& name, UniformType type, glm::vec3& v) : Name(name), Type(type), vec3(v) {}
		Uniform(const std::string& name, UniformType type, glm::vec4& v) : Name(name) , Type(type), vec4(v) {}*/
	};
	
	class DUSK_EXPORT Material : public Asset
	{
	public:
		Material(Handle<Shader> shaderHandle, AssetHandler* owningHandler, const std::filesystem::path& path, const uuids::uuid& uuid);
		//Material(Ref<Shader>& shader, const std::string& name = ""); not used anymore, keeping just in case
		//Material(const std::string& shaderPath, const std::string& name = ""); not used anymore, keeping just in case
		~Material();

		void UploadUniforms(AssetHandler& assetHandler);
		void UniformsDefaultValue();

		void SetShader(const Handle<Shader> shader);
		
		void SetFloat(const std::string& name, float f);
		void SetVec2(const std::string& name, glm::vec2& v);
		void SetVec3(const std::string& name, glm::vec3& v);
		void SetVec4(const std::string& name, glm::vec4& v);
		// change to pass handle only
		void SetTexture(const std::string& name, Ref<Texture>& texture);
		
		const uuids::uuid& GetShaderUUID();
		const uint32_t GetShaderHandle() { return m_ShaderHandle; }
		
		// Serialization
		void SerializeText(const std::string& path);
		static void CreateDefaultMaterial(const std::filesystem::path& path);
	private:
		void CreateUniforms();
		Ref<Shader> m_Shader;

		Handle<Shader> m_ShaderHandle;
		AssetHandler* m_OwningHandler;

		// Map is for direct access to set uniform values
		std::unordered_map<std::string, Uniform*> m_UniformsMap;

		// Vector is where the actual uniform values are located, for iteration - editor drawing and sending to shader
		std::vector<Uniform> m_Uniforms;

		friend class InspectorPanel;
		friend class Scene;
		friend class AssetDatabase;
	};
}