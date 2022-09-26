#include "pch.h"
#include "Logger.h"

#include "Application.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace DuskEngine
{
	std::unordered_map<const char*, Logger*> Logger::m_Loggers;

	Logger::Logger(const char* name)
		:m_Name(name)
	{
		std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_level(spdlog::level::trace);
		consoleSink->set_pattern("%^[%T.%e] [%-9!n] [%-8!l] %v%$");

		std::string logFile = "logs/log_" + Application::Get().GetStartupTime() + ".csv";
		std::shared_ptr<spdlog::sinks::basic_file_sink_mt> fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.c_str());
		fileSink->set_level(spdlog::level::trace);
		fileSink->set_pattern("%v");
		
		m_Logger = std::make_shared<spdlog::logger>(name, fileSink);
		m_Logger->set_level(spdlog::level::trace);
		m_Logger->flush_on(spdlog::level::err);
		
		if(m_Loggers.size() == 0)
		{
			m_Logger->trace("Time,Origin,Level,Message");
			fileSink->flush();
			fileSink->set_pattern("%^%T.%e,%n,%l,%v%$");
		}
		else
			fileSink->set_pattern("%^%T.%e,%n,%l,%v%$");
		
		m_Loggers.insert({ name, this });
		
		auto& sinks = m_Logger->sinks();
		sinks.push_back(consoleSink);
		
		std::string message = "Created Logger ";
		message.append(name); 
		TRACE(message);
	}

	Logger::~Logger()
	{
		std::string message = "Destroyed Logger ";
		message.append(m_Name);
		TRACE(message);
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

	void Logger::Log(const std::string& message, LogLevel level, const char* file, unsigned int line)
	{
		Log(message.c_str(), level, file, line);
	}
}