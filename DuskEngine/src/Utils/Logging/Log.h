#pragma once

#include "spdlog/spdlog.h"

#define DUSK_DEFAULT_LOGGER_NAME "Logger"

#ifdef DUSK_DEBUG
	#define DUSK_LOG_TRACE(...)	if(spdlog::get(DUSK_DEFAULT_LOGGER_NAME) != nullptr) { spdlog::get(DUSK_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);	}
	#define DUSK_LOG_DEBUG(...)	if(spdlog::get(DUSK_DEFAULT_LOGGER_NAME) != nullptr) { spdlog::get(DUSK_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__);	}
	#define DUSK_LOG_INFO(...)	if(spdlog::get(DUSK_DEFAULT_LOGGER_NAME) != nullptr) { spdlog::get(DUSK_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);	}
	#define DUSK_LOG_WARN(...)	if(spdlog::get(DUSK_DEFAULT_LOGGER_NAME) != nullptr) { spdlog::get(DUSK_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);	}
	#define DUSK_LOG_ERROR(...)	if(spdlog::get(DUSK_DEFAULT_LOGGER_NAME) != nullptr) { spdlog::get(DUSK_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);	}
	#define DUSK_LOG_FATAL(...)	if(spdlog::get(DUSK_DEFAULT_LOGGER_NAME) != nullptr) { spdlog::get(DUSK_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);}
#else
	#define DUSK_LOG_TRACE(...)
	#define DUSK_LOG_DEBUG(...)
	#define DUSK_LOG_INFO(...)
	#define DUSK_LOG_WARN(...)
	#define DUSK_LOG_ERROR(...)
	#define DUSK_LOG_FATAL(...)
#endif // DUSK_DEBUG
