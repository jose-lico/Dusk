#include "pch.h"
#include "Time.h"

#include "GLFW/glfw3.h"

namespace DuskEngine::Time
{
	float DeltaTime = 0;
	float Time = 0;
	float UnscaledTime = 0;
	float LastFrame = 0;
	float Timescale = 1;

	void Update()
	{
		float currentFrame = (float)glfwGetTime();
		DeltaTime = currentFrame - LastFrame;
		DeltaTime *= Timescale;
		Time += DeltaTime;
		LastFrame = currentFrame;
		UnscaledTime += DeltaTime/Timescale;
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
		return DeltaTime / Timescale;
	}

	float GetUnscaledTime()
	{
		return UnscaledTime;
	}

	float GetTimescale()
	{
		return Timescale;
	}

	float SetTimescale(float timeScale)
	{
		return Timescale = timeScale;
	}
}
