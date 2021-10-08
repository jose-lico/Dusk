#pragma once

#ifdef DUSK_WINDOWS
	#ifdef DUSK_DLL
		#define DUSK_API __declspec(dllexport)
	#else
		#define DUSK_API __declspec(dllimport)
	#endif
#elif DUSK_LINUX
	#define DUSK_API 
#endif