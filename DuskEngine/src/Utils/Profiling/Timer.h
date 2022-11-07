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

		/*~TimerNode()
		{
			printf("Deleted node %s\n", Name.c_str());
		}*/
	};

	class Timer
	{
	public:
		Timer(const std::string& text, bool runtime = false);
		~Timer();
		inline static TimerNode* GetRootTimer() { return m_RootTimer; }
		inline static void ResetRoot() { m_RootTimer = nullptr; }
		static bool Stop; // very temp 
	private:
		static TimerNode* m_RootTimer;
		static TimerNode* m_LastTimer;

		TimerNode* m_MyNode;
		bool m_Runtime;
		std::string m_Text;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}