#include "pch.h"
#include "Time.h"

#include "GLFW/glfw3.h"

namespace DuskEngine::Time
{
	float DeltaTime = 0;
	float Time = 0;
	float LastFrame = 0;

	void Update()
	{
		float currentFrame = (float)glfwGetTime();
		DeltaTime = currentFrame - LastFrame;
		LastFrame = currentFrame;
	}

	float GetDeltaTime()
	{
		return DeltaTime;
	}
}
