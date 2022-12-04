#pragma once

#include "Logger.h"

#ifdef DUSK_LIB
	#define LOGGER "Dusk"
#else
	#define LOGGER "Editor"
#endif

#define TRACE(...) Logger::Get(LOGGER)->Log(__VA_ARGS__, DuskEngine::LogLevel::TRACE, __FILE__, __LINE__)
#define LOG(...) Logger::Get(LOGGER)->Log(__VA_ARGS__, DuskEngine::LogLevel::LOG, __FILE__, __LINE__)
#define WARN(...) Logger::Get(LOGGER)->Log(__VA_ARGS__, DuskEngine::LogLevel::WARN, __FILE__, __LINE__)
#define ERR(...) Logger::Get(LOGGER)->Log(__VA_ARGS__, DuskEngine::LogLevel::ERR, __FILE__, __LINE__)
#define FATAL(...) Logger::Get(LOGGER)->Log(__VA_ARGS__, DuskEngine::LogLevel::FATAL, __FILE__, __LINE__)

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }