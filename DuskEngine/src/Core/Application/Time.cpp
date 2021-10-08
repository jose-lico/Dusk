#include "pch.h"
#include "Time.h"

#include "GLFW/glfw3.h"

namespace DuskEngine
{
	float Time::m_Time = 0.0f;
	float Time::m_DeltaTime = 0.0f;
	float Time::m_LastFrame = 0.0f;

	void Time::Init()
	{

	}

	void Time::Update()
	{
		float currentFrame = (float)glfwGetTime();
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;
	}
}