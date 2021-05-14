#pragma once

#include <glm.hpp>
#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/Shader.h"

namespace DuskEngine 
{
	class Renderer
	{
	public:
		static Renderer& Get()
		{
			static Renderer instance;
			return instance;
		}

		static void Init();

		static const void Clear();
		static const void ClearColor(const glm::vec4& color);
		static void DrawElements(const VertexArray& va, const Shader& shader);
		static void DrawArrays(const VertexArray& va, const Shader& shader, unsigned int start, unsigned int count);

		struct Stats
		{
			unsigned int DrawCalls = 0;
			unsigned int VertCount = 0;
		};

		static void ResetStats();
		static const Stats GetStats();
	};
}