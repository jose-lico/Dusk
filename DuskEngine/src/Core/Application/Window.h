#pragma once

#include <functional>

struct GLFWwindow;

namespace DuskEngine
{
	class Event;

	struct WindowData
	{
		std::string Title = "Dusk Engine";
		uint16_t Width = 1280;
		uint16_t Height = 720;
		bool VSync = true;
		bool Maximized = true;
		bool FullScreen = false;

		std::function<void(Event&)> EventCallback;
	};

	class Window
	{
	public:
		Window(const WindowData& data);
		~Window();

		GLFWwindow* GetNativeHandle() const { return m_Window; }
		inline uint16_t GetWidth() const { return m_Data.Width; }
		inline uint16_t GetHeight() const { return m_Data.Height; }
		inline const std::string& GetTitle() const { return m_Data.Title; }
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