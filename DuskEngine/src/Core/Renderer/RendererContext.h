#pragma once

#include "Utils/Memory/Memory.h"

namespace DuskEngine 
{
	class RendererContext
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, D3D11 = 2
		};

		virtual ~RendererContext() = default;

		virtual void SwapBuffers() = 0;

		static RendererContext* Create(void* window);
		static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}