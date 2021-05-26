#include "pch.h"

#include "Renderer.h"

namespace DuskEngine
{
	static Renderer::Stats stats;

	void Renderer::Init()
	{
		if (glewInit() != GLEW_OK)
			std::cout << "Error" << std::endl;
	}

	const void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	const void Renderer::ClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::DrawElements(const VertexArray& va, const Shader& shader)
	{
		shader.Bind();
		va.Bind();
		glDrawElements(GL_TRIANGLES, va.Indices, GL_UNSIGNED_INT, nullptr);

		stats.DrawCalls++;
	}

	void Renderer::DrawArrays(const VertexArray& va, const Shader& shader, unsigned int start, unsigned int count)
	{
		shader.Bind();
		va.Bind();
		glDrawArrays(GL_TRIANGLES, start, count);

		stats.DrawCalls++;
		stats.VertCount += count;
	}

	void Renderer::ResetStats()
	{
		stats.DrawCalls = 0;
	}

	const Renderer::Stats Renderer::GetStats()
	{
		return stats;
	}
}