#pragma once

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

namespace DuskEngine
{
	struct OpenGLBuffer;
	struct VertexBuffer;
	struct VertexArray;
	struct Shader;
	struct Texture;
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

	void SetBufferData(OpenGLBuffer& buffer, void* data);
	void BindBuffer(const OpenGLBuffer& buffer);
	void FreeBuffer(OpenGLBuffer& buffer);

	// #############################################################
	// Vertex Array
	// #############################################################

	void GenVertexArrays(VertexArray& id);
	void BindVertexArray(const VertexArray& id);
	void UnbindVertexArray();
	void SetVertexAttribPointer(VertexBuffer& vb);
	void FreeVertexArray(VertexArray& va);

	// #############################################################
	// Texture
	// #############################################################

	void SetTextureData(Texture& texture, const unsigned char* data, bool offset = false);
	void BindTexture(const Texture& texture, uint32_t slot);
	void FreeTexture(Texture& texture);

	// #############################################################
	// Shader
	// #############################################################

	void CreateProgram(Shader& shader, const std::string& vertexShader, const std::string& fragmentShader);
	void UseProgram(const Shader& shader);
	void DeleteProgram(Shader& shader);

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
	void BindFramebuffer(const Framebuffer& buffer);
	void UnbindFramebuffer();
	void FreeFramebuffer(Framebuffer& buffer);

	// #############################################################
	// Drawing
	// #############################################################

	void DrawIndexed(const VertexArray& vertexArray);
	void DrawArrays(unsigned int start, unsigned int count);
}