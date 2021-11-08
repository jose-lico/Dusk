#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define LOGGER_ENGINE "Engine"
#define LOGGER_APP "Application"
#define DEFAULT_PATTERN logger->set_pattern("%^[%T] %n: %v%$");

#if !defined(DUSK_EXE) && (defined(DUSK_DLL) || defined(DUSK_LIB))
	#ifdef DUSK_DEBUG
		#define CREATE_LOGGER\
			{\
				auto logger = spdlog::stdout_color_mt(LOGGER_ENGINE); \
				DEFAULT_PATTERN\
				logger->set_level(spdlog::level::trace); \
			}
	#else
		#define CREATE_LOGGER\
			{\
				auto logger = spdlog::stdout_color_mt(LOGGER_ENGINE); \
				DEFAULT_PATTERN\
				logger->set_level(spdlog::level::debug); \
			}
	#endif

	#define TRACE(...)	spdlog::get(LOGGER_ENGINE)->trace(__VA_ARGS__);
	#define LOG(...)	spdlog::get(LOGGER_ENGINE)->debug(__VA_ARGS__);
	#define INFO(...)	spdlog::get(LOGGER_ENGINE)->info(__VA_ARGS__);
	#define WARN(...)	spdlog::get(LOGGER_ENGINE)->warn(__VA_ARGS__);
	#define ERROR(...)	spdlog::get(LOGGER_ENGINE)->error(__VA_ARGS__);
	#define FATAL(...)	spdlog::get(LOGGER_ENGINE)->critical(__VA_ARGS__);
#else
	#ifdef DUSK_DEBUG
		#define CREATE_APP_LOGGER\
			{\
				auto logger = spdlog::stdout_color_mt(LOGGER_APP); \
				DEFAULT_PATTERN\
				logger->set_level(spdlog::level::trace);\
			}
	#else
		#define CREATE_APP_LOGGER\
			{\
				auto logger = spdlog::stdout_color_mt(LOGGER_APP); \
				DEFAULT_PATTERN\
				logger->set_level(spdlog::level::debug); \
			}
	#endif

	#define APP_TRACE(...)	spdlog::get(LOGGER_APP)->trace(__VA_ARGS__);
	#define APP_LOG(...)	spdlog::get(LOGGER_APP)->debug(__VA_ARGS__);
	#define APP_INFO(...)	spdlog::get(LOGGER_APP)->info(__VA_ARGS__);
	#define APP_WARN(...)	spdlog::get(LOGGER_APP)->warn(__VA_ARGS__);
	#define APP_ERROR(...)	spdlog::get(LOGGER_APP)->error(__VA_ARGS__);
	#define APP_FATAL(...)	spdlog::get(LOGGER_APP)->critical(__VA_ARGS__);
#endif