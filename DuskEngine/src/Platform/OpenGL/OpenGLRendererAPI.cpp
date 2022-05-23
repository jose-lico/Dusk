#include "pch.h"
#include "OpenGLRendererAPI.h"

#include "GLCommon.h"

#include "Core/Renderer/Resources/VertexArray.h"
#include "Core/Renderer/Resources/Buffer.h"


namespace DuskEngine {

	void OpenGLRendererAPI::Init()
	{
		TRACE("OpenGL API Created");
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::UnbindVAO()
	{
		glBindVertexArray(0);
	}

	void OpenGLRendererAPI::DrawArrays(unsigned int start, unsigned int count)
	{
		glDrawArrays(GL_TRIANGLES, start, count);
	}
}