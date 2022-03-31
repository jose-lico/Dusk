#pragma once
#include "Utils/Window/Window.h"

struct GLFWwindow;

namespace DuskEngine
{
	class WindowsWindow : public Window 
	{
	public:
		WindowsWindow(const WindowData& data = WindowData());
		~WindowsWindow();

		virtual unsigned int GetWidth() const override { return m_Data.Width; }
		virtual unsigned int GetHeight() const override { return m_Data.Height; }
		virtual bool IsVSync() const override;

		virtual void SetEventCallback(const std::function<void(Event&)>& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool vsync) override;

		virtual bool ShouldClose() const override;

		virtual void* GetNativeHandle() const override;
	private:
		GLFWwindow* m_Window;
	};
}