#pragma once

#include "Logger.h"

#ifdef DUSK_LIB
	#define LOGGER "Dusk"
	#define LOG2(...) Logger::Get(LOGGER)->Log(__VA_ARGS__, DuskEngine::LogLevel::TRACE, __FILE__, __LINE__)
#else
	DuskEngine::Logger* g_EditorLogger;
#endif