#pragma once
#include "Core/Macros/DUSK_API.h"

namespace DuskEngine
{
	class DUSK_EXPORT Time
	{
	public:
		static void Init();
		static void Update();
		static float GetDeltaTime() { return m_DeltaTime; }
		static float GetTime() { return m_Time; }
	private:
		static float m_DeltaTime;
		static float m_Time;
		static float m_LastFrame;
	};
}