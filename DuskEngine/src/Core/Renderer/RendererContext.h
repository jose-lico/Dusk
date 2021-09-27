#pragma once

namespace DuskEngine 
{
	class RendererContext
	{
	public:
		virtual void Init() = 0;
		virtual void SetVSync(bool vsync) = 0;
		virtual void SwapBuffers() = 0;

		static RendererContext* Create(void* window);
	};
}