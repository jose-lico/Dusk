#include "pch.h"
#include "Logger.h"

#include "OS.h"
#include "Core.h"
#include "Application.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace DuskEngine
{
	std::unordered_map<std::string_view, Logger*> Logger::m_Loggers;

	Logger::Logger(const char* name)
		:m_Name(name)
	{
		m_Loggers[m_Name] = this;

		bool consoleAttached = Application::Get().GetOS().IsConsoleAttached();
		bool logToFile = Application::Get().GetCliOptions().DumpLogs;

		// If the console is not attached and logs are not written to a file, dont bother to create spdlog logger
		if (!consoleAttached && !logToFile)
			return;
		
		m_Logger = std::make_shared<spdlog::logger>(m_Name);

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
		
		bool verbose = Application::Get().GetCliOptions().Verbose;

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
		
		m_Logger->sinks().push_back(consoleSink);
		
		TRACE("Created Logger {}", m_Name)
	}

	Logger::~Logger()
	{
		TRACE("Destroyed Logger {}", m_Name)
	}

	Logger* Logger::Get(const char* name)
	{
		if (m_Loggers.find(name) != m_Loggers.end())
			return m_Loggers[name];

		// should probably assert
		return nullptr;
	}
}