#include "pch.h"
#include "OpenGLAPI.h"

#include "GLCommon.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Texture.h"

namespace DuskEngine :: OpenGLAPI 
{
	void Init()
	{
		TRACE("OpenGL API Created");
	}

	void SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void SetBufferData(uint32_t& id, BufferType buffer, UsageType usage, void* data, size_t size)
	{
		glGenBuffers(1, &id);
		glBindBuffer((GLenum)buffer, id);
		glBufferData((GLenum)buffer, size, data, (GLenum)usage);
	}

	void BindBuffer(BufferType BufferType, uint32_t id)
	{
		glBindBuffer((GLenum)BufferType, id);
	}

	void FreeBuffer(uint32_t& id)
	{
		glDeleteBuffers(1, &id);
	}

	void SetTextureData(Texture& textureData, const unsigned char* data, bool offset)
	{
		glGenTextures(1, &textureData.ResourceID);
		glBindTexture((GLenum)textureData.Type, textureData.ResourceID);

		glTexParameteri((GLenum)textureData.Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri((GLenum)textureData.Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri((GLenum)textureData.Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri((GLenum)textureData.Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D((GLenum)textureData.Type, 0, (GLenum)textureData.Format, textureData.Width, textureData.Height, 0, 
			(GLenum)textureData.Format, GL_UNSIGNED_BYTE, data + (offset ? sizeof(TextureData) : 0));

		if(textureData.Mipmaps)
			glGenerateMipmap((GLenum)textureData.Type);
	}

	void BindTexture(uint32_t slot, uint32_t id)
	{
		glBindTextureUnit(slot, id);
	}

	void FreeTexture(uint32_t& id)
	{
		glDeleteTextures(1, &id);
	}

	void UnbindVAO()
	{
		glBindVertexArray(0);
	}

	void DrawIndexed(const VertexArray& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray.GetIndexBuffer().Count, GL_UNSIGNED_INT, nullptr);
	}

	void DrawArrays(unsigned int start, unsigned int count)
	{
		glDrawArrays(GL_TRIANGLES, start, count);
	}
}