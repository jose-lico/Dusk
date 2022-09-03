#pragma once

// TODO: Change time class to a more robust clock class
namespace DuskEngine::Time
{
	void Update();

	float GetDeltaTime();
	float GetTime();
	float GetUnscaledDeltaTime();
	float GetUnscaledTime();
	float GetTimescale();
	float SetTimescale(float timeScale);
}