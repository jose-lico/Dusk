#pragma once

#include <functional>

struct GLFWwindow;

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

		WindowData(const std::string& title = "Dusk Engine", unsigned int width = 1920, unsigned height = 1017, bool vsync = true)
			: Title(title), Width(width), Height(height), VSync(vsync)
		{
		}
	};

	class Window
	{
	public:
		Window(const WindowData& data = WindowData());
		~Window() = default;

		GLFWwindow* GetNativeHandle() const { return m_Window; }
		inline unsigned int GetWidth() const { return m_Data.Width; }
		inline unsigned int GetHeight() { return m_Data.Height; }
		inline bool IsVSync() const { return m_Data.VSync; }

		inline void SetEventCallback(const std::function<void(Event&)>& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool vsync); 
		void SetWindowTitle(const std::string& title);
		bool ShouldClose() const;
	private:
		GLFWwindow* m_Window;

		WindowData m_Data;
	};
}