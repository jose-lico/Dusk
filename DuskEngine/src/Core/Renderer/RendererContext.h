#pragma once

namespace DuskEngine 
{
	class RendererContext
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, D3D11 = 2
		};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void SetVSync(bool vsync) = 0;
		virtual void SwapBuffers() = 0;

		static RendererContext* Create(void* window);
		static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}