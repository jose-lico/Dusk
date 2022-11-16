#pragma once
#include "Core/Assets/Asset.h"

#include "Core/Assets/Handle.h"

#include "glm/glm.hpp"

#include <vector>
#include <string>

namespace DuskEngine
{
	struct Shader;
	struct Texture;
	class AssetHandler;
	enum class UniformType;

	enum class MaterialType
	{
		Default,
		Custom
	};

	union UniformData
	{
		float fValue;
		uint32_t dataHandle;
		glm::vec2 vec2;
		glm::vec3 vec3;
		glm::vec4 vec4;
	};

	struct Uniform
	{
		std::string Name;
		UniformType Type;
		
		UniformData Data;
	};
	
	class Material : public Asset
	{
	public:
		Material(Handle<Shader> shaderHandle, AssetHandler* owningHandler, const std::filesystem::path& path, const uuids::uuid& uuid);
		Material(AssetHandler* owningHandler);
		~Material();

		void UploadUniforms(AssetHandler& assetHandler);
		void UniformsDefaultValue();

		void SetShader(const Handle<Shader> shader);
		
		void SetFloat(const std::string& name, float f);
		void SetVec2(const std::string& name, glm::vec2& v);
		void SetVec3(const std::string& name, glm::vec3& v);
		void SetVec4(const std::string& name, glm::vec4& v);
		// change to pass handle only
		void SetTexture(const std::string& name, Texture& texture);
		
		const uint32_t GetShaderHandle() { return m_ShaderHandle; }
		const MaterialType GetType() { return m_Type; }
		
		// Serialization
		void SerializeText(const std::string& path);
		static void CreateDefaultMaterial(const std::filesystem::path& path);
		static Material CreateDefaultMaterial(AssetHandler* owningHandler);
	private:
		void CreateUniforms();

		Handle<Shader> m_ShaderHandle;
		MaterialType m_Type;
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