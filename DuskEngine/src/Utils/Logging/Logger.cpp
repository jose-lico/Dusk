#include "pch.h"
#include "Logger.h"

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace DuskEngine 
{
	void Logger::Init()
	{
		auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		sink->set_pattern("%^[%H:%M:%S.%e] %v%$");

		std::vector<spdlog::sink_ptr> sinks { sink };
		auto logger = std::make_shared<spdlog::logger>(DUSK_DEFAULT_LOGGER_NAME, sinks.begin(), sinks.end());

		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
		spdlog::register_logger(logger);
	}

	void Logger::Shutdown()
	{
		spdlog::shutdown();
	}
}