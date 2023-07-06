#pragma once

#include "Application.h"
#include "OS.h"

#include "spdlog/spdlog.h"

#include <unordered_map>
#include <memory>
#include <string_view>

namespace DuskEngine
{
	enum class LogLevel
	{
		TRACE,
		LOG,
		WARN,
		ERR,
		FATAL
	};

	class Logger
	{
	public:
		Logger(const char* name);
		~Logger();

		static Logger* Get(const char* name);
		
		template <typename... Args>
		void Log(LogLevel level, const char* file, unsigned int line, const char* message, Args&&... args)
		{
			bool consoleAttached = Application::Get().GetOS().IsConsoleAttached();
			bool logToFile = Application::Get().GetCliOptions().DumpLogs;

			// If the console is not attached and logs are not written to a file, dont call spdlog
			if (!consoleAttached && !logToFile)
				return;

			switch (level)
			{
			case LogLevel::TRACE:
				m_Logger->trace(message, std::forward<Args>(args)...);
				break;
			case LogLevel::LOG:
				m_Logger->debug(message, std::forward<Args>(args)...);
				break;
			case LogLevel::WARN:
				m_Logger->warn(message, std::forward<Args>(args)...);
				break;
			case LogLevel::ERR:
				m_Logger->error(message, std::forward<Args>(args)...);
				break;
			case LogLevel::FATAL:
				m_Logger->critical(message, std::forward<Args>(args)...);
				break;
			default:
				break;
			}
		}
	private:
		static std::unordered_map<std::string_view, Logger*> m_Loggers;
		std::shared_ptr<spdlog::logger> m_Logger;
		const char* m_Name;
	};
}