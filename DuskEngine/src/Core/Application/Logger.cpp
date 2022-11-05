#include "pch.h"
#include "Logger.h"

#include "Application.h"
#include "OS.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace DuskEngine
{
	std::unordered_map<const char*, Logger*> Logger::m_Loggers;

	Logger::Logger(const char* name)
		:m_Name(name)
	{
		m_Loggers.insert({ name, this });

		bool consoleAttached = Application::Get().GetOS().IsConsoleAttached();
		bool logToFile = Application::Get().GetCmdOptions().DumpLogs;

		// If the console is not attached and logs are not written to a file, dont bother to create spdlog logger
		if (!consoleAttached && !logToFile)
			return;
		
		m_Logger = std::make_shared<spdlog::logger>(name);

		// If the console is attached, create a sink for it
		std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink;
		if(consoleAttached)
		{
			consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			consoleSink->set_level(spdlog::level::trace);
			consoleSink->set_pattern("%^[%T.%e] [%-9!n] [%-8!l] %v%$");
		}

		// If logging to a file, create a sink for it
		std::shared_ptr<spdlog::sinks::basic_file_sink_mt> fileSink;
		if(logToFile)
		{
			std::string logFile = "logs/log_" + Application::Get().GetStartupTime() + ".csv";
			fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.c_str());
			fileSink->set_level(spdlog::level::trace);
			fileSink->set_pattern("%v");
			m_Logger->sinks().push_back(fileSink);
		}
		
		bool verbose = Application::Get().GetCmdOptions().Verbose;

		// Debug mode is always verbose btw
		if(verbose)
		{
			m_Logger->set_level(spdlog::level::trace);
			m_Logger->flush_on(spdlog::level::trace);
		}
		else
		{
			m_Logger->set_level(spdlog::level::debug);
			m_Logger->flush_on(spdlog::level::debug);
		}

		if(m_Loggers.size() == 1 && logToFile)
		{
			m_Logger->debug("Time,Origin,Level,Message");
			fileSink->flush();
			fileSink->set_pattern("%^%T.%e,%n,%l,%v%$");
		}
		else if(logToFile)
			fileSink->set_pattern("%^%T.%e,%n,%l,%v%$");
		
		m_Logger->sinks().push_back(consoleSink);
		
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
#if DUSK_LINUX
	return;
#endif
		// construct struct with message and save for later, editor stuff
		// In editor mode, this message is always constructed to be displayed in the editor
		// At runtime, dont bother

		bool consoleAttached = Application::Get().GetOS().IsConsoleAttached();
		bool logToFile = Application::Get().GetCmdOptions().DumpLogs;

		// If the console is not attached and logs are not written to a file, dont call spdlog
		if (!consoleAttached && !logToFile)
			return;

		switch (level)
		{
		case LogLevel::TRACE:
			m_Logger->trace(message);
			break;
		case LogLevel::LOG:
			m_Logger->debug(message);
			break;
		case LogLevel::WARN:
			m_Logger->warn(message);
			break;
		case LogLevel::ERR:
			m_Logger->error(message);
			break;
		case LogLevel::FATAL:
			m_Logger->critical(message);
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