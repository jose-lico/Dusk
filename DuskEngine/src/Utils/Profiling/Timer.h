#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace DuskEngine
{
	class ScopeTimer
	{
	public:
		ScopeTimer(const std::string& name);
		~ScopeTimer();
	private:
		std::string m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}