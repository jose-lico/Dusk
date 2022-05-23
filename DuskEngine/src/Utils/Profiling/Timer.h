#pragma once
#include "Core/Application/Core.h"

#include <chrono>
#include <string>

namespace DuskEngine
{
	class Timer
	{
	public:
		Timer(const std::string& text)
			:m_Text(text), m_Start(std::chrono::high_resolution_clock::now()), m_Duration(0.0f)
		{
		}

		~Timer()
		{
			auto end = std::chrono::high_resolution_clock::now();
			m_Duration = end - m_Start;
			
			std::string message = m_Text + " timer took: " + std::to_string(m_Duration.count()) + " seconds";
			TRACE(message.c_str());
			message = m_Text + " timer took: " + std::to_string((m_Duration.count() * 1000.0f)) + " ms";
			TRACE(message.c_str());
		}
	private:
		std::string m_Text;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		std::chrono::duration<float> m_Duration;
	};
}