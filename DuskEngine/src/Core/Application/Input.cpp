#include "pch.h"
#include "Input.h"

#include "Core/Application/Application.h"

#include "Utils/Window/Window.h"

#include "GLFW/glfw3.h"

namespace DuskEngine
{
	bool Input::IsKeyPressed(const KeyCode keycode)
	{
		auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeHandle();
		if (glfwGetKey(window, keycode) == GLFW_PRESS)
			return true;
		return false;
	}

	bool Input::IsMouseButtonPressed(const MouseCode mousecode)
	{
		auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeHandle();
		if (glfwGetMouseButton(window, mousecode) == GLFW_PRESS)
			return true;
		return false;
	}

	float Input::GetMouseX()
	{
		auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeHandle();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return (float)xpos;
	}

	float Input::GetMouseY()
	{
		auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeHandle();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return (float)ypos;
	}

	void Input::SetCursorActive(CursorState state)
	{
		auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeHandle();
		glfwSetInputMode(window, GLFW_CURSOR, state);
	}
}