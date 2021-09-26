#pragma once

#include <glm.hpp>
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/Shader.h"

namespace DuskEngine 
{
	class OpenGLRenderer
	{
	public:
		static OpenGLRenderer& Get()
		{
			static OpenGLRenderer instance;
			return instance;
		}

		static void Init();

		static const void Clear();
		static const void ClearColor(const glm::vec4& color);
		static void DrawElements(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader);
		static void DrawArrays(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader, unsigned int start, unsigned int count);

		struct Stats
		{
			unsigned int DrawCalls = 0;
			unsigned int VertCount = 0;
		};

		static void ResetStats();
		static const Stats GetStats();
	};
}