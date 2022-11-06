#include "pch.h"
#include "Timer.h"

#include "Core/Application/Core.h"

namespace DuskEngine
{
	TimerNode* Timer::m_RootTimer = nullptr;
	TimerNode* Timer::m_LastTimer = nullptr;
	bool Timer::Stop = false;

	Timer::Timer(const std::string& text, bool runtime)
	:m_Text(text), m_Start(std::chrono::high_resolution_clock::now())
	{
		if(!Stop)
		{
			m_MyNode = new TimerNode;
			m_MyNode->Name = m_Text;

			if (m_RootTimer == nullptr)
				m_RootTimer = m_MyNode;
			
			if(m_LastTimer != nullptr)
			{
				m_LastTimer->Children.push_back(m_MyNode);
				m_MyNode->Parent = m_LastTimer;
			}


			m_LastTimer = m_MyNode;
		}
	}
	
	Timer::~Timer()
	{
		if(!Stop)
		{
			auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start);

			TRACE(m_Text + " took: " + std::to_string(duration.count() / 1000000.0f) + " ms");

			m_MyNode->Duration = duration;
			if(m_MyNode->Parent != nullptr)
				m_LastTimer = m_MyNode->Parent;
		}
	}
}
