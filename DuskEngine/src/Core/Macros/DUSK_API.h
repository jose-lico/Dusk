#pragma once

#ifdef DUSK_WINDOWS
	#if defined(DUSK_EXE) || defined(DUSK_LIB)
		#define DUSK_EXPORT 
	#else
		#ifdef DUSK_DLL
			#define DUSK_EXPORT __declspec(dllexport)
		#else
			#define DUSK_EXPORT __declspec(dllimport)
		#endif
	#endif
#elif DUSK_LINUX
	#define DUSK_EXPORT 
#endif