#include "pch.h"
#include "OpenGLRenderer.h"

#include "GLCommon.h"

namespace DuskEngine
{
	static OpenGLRenderer::Stats stats;

	void OpenGLRenderer::Init()
	{
		if (glewInit() != GLEW_OK)
			std::cout << "Error" << std::endl;
	}

	const void OpenGLRenderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	}

	const void OpenGLRenderer::ClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderer::DrawElements(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();
		va->Bind();
		glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

		stats.DrawCalls++;
	}

	void OpenGLRenderer::DrawArrays(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader, unsigned int start, unsigned int count)
	{
		shader->Bind();
		va->Bind();
		glDrawArrays(GL_TRIANGLES, start, count);

		stats.DrawCalls++;
		stats.VertCount += count;
	}

	void OpenGLRenderer::ResetStats()
	{
		stats.DrawCalls = 0;
	}

	const OpenGLRenderer::Stats OpenGLRenderer::GetStats()
	{
		return stats;
	}
}