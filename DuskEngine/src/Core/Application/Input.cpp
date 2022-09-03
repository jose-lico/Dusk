#include "pch.h"
#include "Input.h"

#include "Application.h"
#include "Window.h"

#include "GLFW/glfw3.h"

namespace DuskEngine::Input
{
	bool IsKeyPressed(const KeyCode keycode)
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeHandle();
		if (glfwGetKey(window, keycode) == GLFW_PRESS)
			return true;
		return false;
	}

	bool IsMouseButtonPressed(const MouseCode mousecode)
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeHandle();
		if (glfwGetMouseButton(window, mousecode) == GLFW_PRESS)
			return true;
		return false;
	}

	float GetMouseX()
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeHandle();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return (float)xpos;
	}

	float GetMouseY()
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeHandle();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return (float)ypos;
	}

	void SetCursorActive(CursorState state)
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeHandle();
		glfwSetInputMode(window, GLFW_CURSOR, state);
	}
}