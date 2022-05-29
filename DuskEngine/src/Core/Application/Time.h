#pragma once

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