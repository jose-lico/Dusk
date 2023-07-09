#pragma once

#include "Logger.h"

#ifdef DUSK_LIB
	#define LOGGER "Dusk"
#else
	#define LOGGER "Editor"
#endif

#define TRACE(...) Logger::Get(LOGGER)->Log(DuskEngine::LogLevel::TRACE, __FILE__, __LINE__, __VA_ARGS__);
#define LOG(...) Logger::Get(LOGGER)->Log(DuskEngine::LogLevel::LOG, __FILE__, __LINE__, __VA_ARGS__);
#define WARN(...) Logger::Get(LOGGER)->Log(DuskEngine::LogLevel::WARN, __FILE__, __LINE__, __VA_ARGS__);
#define ERR(...) Logger::Get(LOGGER)->Log(DuskEngine::LogLevel::ERR, __FILE__, __LINE__, __VA_ARGS__);
#define FATAL(...) Logger::Get(LOGGER)->Log(DuskEngine::LogLevel::FATAL, __FILE__, __LINE__, __VA_ARGS__);

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef DUSK_DEBUG
	#ifdef DUSK_WINDOWS
		#define ASSERT(x) \
			if(x) {} \
			else \
			{ \
				__debugbreak(); \
			}
	#elif DUSK_LINUX
		#define ASSERT(x)
	#endif
#endif