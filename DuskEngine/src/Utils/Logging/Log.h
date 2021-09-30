#pragma once

#include "spdlog/spdlog.h"

#define DUSK_DEFAULT_LOGGER_NAME "Logger"

#ifdef DUSK_DEBUG
	#define DUSK_LOG(...) if(spdlog::get(DUSK_DEFAULT_LOGGER_NAME) != nullptr) { spdlog::get(DUSK_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__); }
#else
	#define DUSK_LOG(...)
#endif // DUSK_DEBUG
