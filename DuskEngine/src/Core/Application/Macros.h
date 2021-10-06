#pragma once

#include <memory>

#ifdef DUSK_WINDOWS
	#ifdef DUSK_DLL
		#define DUSK_API __declspec(dllexport)
	#else
		#define DUSK_API __declspec(dllimport)
	#endif
#elif DUSK_LINUX
	#define DUSK_API 
#endif

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }