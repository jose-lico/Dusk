#pragma once

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

namespace DuskEngine
{
	struct VertexArray;
	struct Texture;
	struct Shader;
	struct VertexBuffer;
	struct Framebuffer;
	enum class BufferType : uint16_t;
	enum class UsageType : uint16_t;
}

namespace DuskEngine :: OpenGLAPI
{
	void Init();
	void SetClearColor(const glm::vec4& color);
	void Clear();

	// #############################################################
	// Buffer
	// #############################################################

	void SetBufferData(uint32_t& id, BufferType buffer, UsageType usage, void* data, size_t size);
	void BindBuffer(BufferType BufferType, uint32_t id);
	void FreeBuffer(uint32_t& id);

	// #############################################################
	// Vertex Array
	// #############################################################

	void GenVertexArrays(uint32_t& id);
	void BindVertexArray(uint32_t id);
	void UnbindVertexArray();
	void SetVertexAttribPointer(VertexBuffer& vb);
	void FreeVertexArray(VertexArray& va);

	// #############################################################
	// Texture
	// #############################################################

	void SetTextureData(Texture& textureData, const unsigned char* data, bool offset = false);
	void BindTexture(uint32_t slot, uint32_t id);
	void FreeTexture(uint32_t& id);

	// #############################################################
	// Shader
	// #############################################################

	void CreateProgram(Shader& shader, const std::string& vertexShader, const std::string& fragmentShader);
	void UseProgram(uint32_t id);
	void DeleteProgram(uint32_t id);

	void SetUniformInt(Shader& shader, const std::string& name, int i);
	void SetUniformFloat(Shader& shader, const std::string& name, float f);
	void SetUniformVec2(Shader& shader, const std::string& name , const glm::vec2& v);
	void SetUniformVec3(Shader& shader, const std::string& name , const glm::vec3& v);
	void SetUniformVec4(Shader& shader, const std::string& name , const glm::vec4& v);
	void SetUniformMat3(Shader& shader, const std::string& name , const glm::mat3& m);
	void SetUniformMat4(Shader& shader, const std::string& name , const glm::mat4& m);

	// #############################################################
	// Framebuffer
	// #############################################################

	void ResizeFramebuffer(Framebuffer& buffer);
	void BindFramebuffer(Framebuffer& buffer);
	void UnbindFramebuffer();
	void FreeFramebuffer(Framebuffer& buffer);

	// #############################################################
	// Drawing
	// #############################################################

	void DrawIndexed(const VertexArray& vertexArray);
	void DrawArrays(unsigned int start, unsigned int count);
}