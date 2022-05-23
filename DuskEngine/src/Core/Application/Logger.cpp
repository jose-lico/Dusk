#include "pch.h"
#include "Logger.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#undef ERROR

namespace DuskEngine
{
	std::unordered_map<const char*, Logger*> Logger::m_Loggers;

	Logger::Logger(const char* name)
	{
		m_Logger = spdlog::stdout_color_mt(name);
		m_Logger->set_level(spdlog::level::trace);
		m_Logger->set_pattern("%^[%T] [%n]: %v%$");

		m_Loggers.insert({ name, this });
	}

	Logger::~Logger()
	{
		
	}

	Logger* Logger::Get(const char* name)
	{
		if (m_Loggers.find(name) != m_Loggers.end())
			return m_Loggers[name];
	}

	void Logger::Log(const char* message, LogLevel level, const char* file, unsigned int line)
	{
		// construct struct with message and save for later, editor stuff

		// rework this & omit once editor has this information
		std::string log = "File: ";
		log.append(file); // Should remove the whole C blah blah
		log.append(" Line:");
		log.append(std::to_string(line));
		log.append(" ");
		log = "";
		log.append(message);

		switch (level)
		{
		case LogLevel::TRACE:
			m_Logger->trace(log.c_str());
			break;
		case LogLevel::LOG:
			m_Logger->debug(log.c_str());
			break;
		case LogLevel::WARN:
			m_Logger->warn(log.c_str());
			break;
		case LogLevel::ERROR:
			m_Logger->error(log.c_str());
			break;
		case LogLevel::FATAL:
			m_Logger->critical(log.c_str());
			break;
		default:
			break;
		}
	}
}