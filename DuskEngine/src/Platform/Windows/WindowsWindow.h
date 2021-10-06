#pragma once
#include "Utils/Window/Window.h"

#include "Core/Renderer/RendererContext.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace DuskEngine
{
	class WindowsWindow : public Window 
	{
	public:
		WindowsWindow() = default;
		~WindowsWindow() = default;

		void Init(const WindowData& data = WindowData()) override;
		void Shutdown() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }
		bool IsVSync() const override;

		void SetEventCallback(const std::function<void(Event&)>& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool vsync) override;

		bool ShouldClose() const override;

		void* GetNativeHandle() const override;
	private:
		GLFWwindow* m_Window;
		WindowData m_Data;
	};
}