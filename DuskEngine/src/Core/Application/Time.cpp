#include "pch.h"
#include "Time.h"

#include "GLFW/glfw3.h"

namespace DuskEngine::Time
{
	float DeltaTime = 0;
	float Time = 0;
	float UnscaledTime = 0;
	float LastFrame = 0;
	float TimeScale = 1;

	void Update()
	{
		float currentFrame = (float)glfwGetTime();
		DeltaTime = currentFrame - LastFrame;
		DeltaTime *= TimeScale;
		Time += DeltaTime;
		LastFrame = currentFrame;
		UnscaledTime += DeltaTime/TimeScale;
	}

	float GetDeltaTime()
	{
		return DeltaTime;
	}

	float GetTime()
	{
		return Time;
	}

	float GetUnscaledDeltaTime()
	{
		return DeltaTime / TimeScale;
	}

	float GetUnscaledTime()
	{
		return UnscaledTime;
	}
}
