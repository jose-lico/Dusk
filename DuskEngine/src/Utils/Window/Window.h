#pragma once

#include <functional>

#include "Core/Renderer/RendererContext.h"

namespace DuskEngine
{
	class Event;

	struct WindowData
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		std::function<void(Event&)> EventCallback;

		WindowData(const std::string& title = "Dusk Engine", unsigned int width = 1080, unsigned height = 720, bool vsync = true)
			: Title(title), Width(width), Height(height), VSync(vsync)
		{
		}
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual void SetVSync(bool vsync) = 0;

		virtual bool ShouldClose() const = 0;

		virtual void* GetNativeHandle() const = 0;
	protected:
		WindowData m_Data;
	};
}