#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace DuskEngine
{
	struct TimerNode {
		std::chrono::nanoseconds Duration;

		std::string Name;
		TimerNode* Parent;
		std::vector<TimerNode*> Children;
	};

	class Timer
	{
	public:
		Timer(const std::string& text);
		~Timer();
		inline static TimerNode* GetRootTimer() { return m_RootTimer; }
	private:
		static TimerNode* m_RootTimer;
		static TimerNode* m_LastTimer;

		TimerNode* m_MyNode;
		std::string m_Text;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}