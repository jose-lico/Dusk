#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace DuskEngine
{
	class Timer
	{
	public:
		Timer(const std::string& text)
			:m_Text(text)
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			auto end = std::chrono::high_resolution_clock::now();
			duration = end - m_Start;

			std::cout << m_Text << " timer took: " << duration.count() << " seconds" << std::endl;
			std::cout << m_Text << " timer took: " << (duration.count() * 1000.0f) << " ms" << std::endl;
		}
	private:
		std::string m_Text;
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
		std::chrono::duration<float> duration;
	};
}