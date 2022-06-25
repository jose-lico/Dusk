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

	void UnbindVAO()
	{
		glBindVertexArray(0);
	}

	void DrawIndexed(const UniqueRef<DuskEngine::VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void DrawArrays(unsigned int start, unsigned int count)
	{
		glDrawArrays(GL_TRIANGLES, start, count);
	}
}