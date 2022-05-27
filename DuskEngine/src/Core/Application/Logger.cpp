#include "pch.h"
#include "Logger.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace DuskEngine
{
	std::unordered_map<const char*, Logger*> Logger::m_Loggers;

	Logger::Logger(const char* name)
		:m_Name(name)
	{
		m_Logger = spdlog::stdout_color_mt(name);
		m_Logger->set_level(spdlog::level::trace);
		m_Logger->set_pattern("%^[%T] [%n]: %v%$");

		m_Loggers.insert({ name, this });
		
		std::string message = "Created Logger ";
		message.append(name);
		LOG(message.c_str());
	}

	Logger::~Logger()
	{
		std::string message = "Destroyed Logger ";
		message.append(m_Name);
		LOG(message.c_str());
	}

	Logger* Logger::Get(const char* name)
	{
		// for some godforsaken reason, unordered_map.find(name) on gcc 
		// was finding the entry but returning a nullptr, this works :/    
		for (auto const& pair : m_Loggers) {
			if (strcmp(name, pair.first) == 0)
				return pair.second;
		}

		return nullptr;
	}

	void Logger::Log(const char* message, LogLevel level, const char* file, unsigned int line)
	{
		// construct struct with message and save for later, editor stuff

		// Omitted for now
		std::string log = "File: ";
		log.append(file);
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
		case LogLevel::ERR:
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