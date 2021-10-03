#pragma once

#include "Utils/Window/Window.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Core/Renderer/RendererContext.h"

namespace DuskEngine
{
	class WindowsWindow : public Window 
	{
	public:
		WindowsWindow() {};
		~WindowsWindow() {};

		void Init(const WindowData& data = WindowData()) override;
		void Shutdown() override;
		void OnUpdate(RendererContext* context) override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetVSync(bool vsync) override;
		bool IsVSync() const override;

		bool ShouldClose() const override;

		void* GetNativeHandle() const override;
	private:
		GLFWwindow* m_Window;
		WindowData m_Data;
	};
}