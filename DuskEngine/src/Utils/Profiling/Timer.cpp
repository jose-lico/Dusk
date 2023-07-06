#include "pch.h"
#include "Timer.h"

#include "Core/Application/Core.h"

namespace DuskEngine
{
	ScopeTimer::ScopeTimer(const std::string& name)
	:m_Name(name)
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}
	
	ScopeTimer::~ScopeTimer()
	{
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start);

		TRACE("{} took: {} ms", m_Name, duration.count()/1000000.0f);
	}
}
