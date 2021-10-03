#pragma once

namespace DuskEngine 
{
	class Logger
	{
	public:
		Logger() = default;
		~Logger() = default;

		void Init();
		void Shutdown();
	};
}