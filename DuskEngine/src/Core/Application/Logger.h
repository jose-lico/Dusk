#pragma once

#include "Utils/Memory/Memory.h"

#include <unordered_map>

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
		ERROR,
		FATAL
	};

	class Logger
	{
	public:
		Logger(const char* name);
		~Logger();

		static Logger* Get(const char* name);

		void Log(const char* message, LogLevel level, const char* file, unsigned int line);
	private:
		static std::unordered_map<const char*, Logger*> m_Loggers;
		Ref<spdlog::logger> m_Logger;
	};
}