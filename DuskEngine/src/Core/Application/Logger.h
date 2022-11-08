#pragma once

#include <unordered_map>
#include <memory>
#include <string_view>

namespace spdlog
{
	class logger;
}

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

		void Log(const char* message, LogLevel level, const char* file, unsigned int line);
		void Log(const std::string& message, LogLevel level, const char* file, unsigned int line);
	private:
		static std::unordered_map<std::string_view, Logger*> m_Loggers;
		std::shared_ptr<spdlog::logger> m_Logger;
		const char* m_Name;
	};
}