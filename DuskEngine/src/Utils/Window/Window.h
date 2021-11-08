#pragma once

#include "pch.h"

#include "Core/Renderer/RendererContext.h"
#include "Core/Events/Event.h"

namespace DuskEngine
{
	struct WindowData
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		std::function<void(Event&)> EventCallback;

		WindowData(const std::string& title = "Window", unsigned int width = 1920, unsigned height = 1080, bool vsync = true)
			: Title(title), Width(width), Height(height), VSync(vsync)
		{
		}
	};

	class Window
	{
	public:
		virtual void Init(const WindowData& data = WindowData()) = 0;
		virtual void Shutdown() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual void SetVSync(bool vsync) = 0;

		virtual bool ShouldClose() const = 0;

		virtual void* GetNativeHandle() const = 0;
	};
}