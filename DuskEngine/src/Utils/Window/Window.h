#pragma once

#include "pch.h"

#include "Core/Renderer/RendererContext.h"

namespace DuskEngine
{
	struct WindowData
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		WindowData(const std::string& title = "Window", unsigned int width = 1280, unsigned height = 720, bool vsync = true)
			: Title(title), Width(width), Height(height), VSync(vsync)
		{
		}
	};

	class Window
	{
	public:
		virtual void Init(const WindowData& data = WindowData()) = 0;
		virtual void Shutdown() = 0;

		virtual void OnUpdate(RendererContext* context) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetVSync(bool vsync) = 0;
		virtual bool IsVSync() const = 0;

		virtual bool ShouldClose() const = 0;

		virtual void* GetNativeHandle() const = 0;
	};
}