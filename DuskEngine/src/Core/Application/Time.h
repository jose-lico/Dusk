#pragma once

#include "Core/Application/Macros.h"

namespace DuskEngine
{
	class DUSK_API Time 
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