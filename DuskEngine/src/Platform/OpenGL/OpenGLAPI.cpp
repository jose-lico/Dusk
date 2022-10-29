#include "pch.h"
#include "OpenGLAPI.h"

#include "GLCommon.h"
#include "VertexArray.h"
#include "Buffer.h"

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