#include "pch.h"
#include "OS.h"

#ifdef DUSK_WINDOWS
#include "Platform/Windows/OS_Windows.h"
#elif DUSK_LINUX
#include "Platform/Linux/OS_Linux.h"
#endif

namespace DuskEngine
{
	OS* DuskEngine::OS::Create()
	{
#ifdef DUSK_WINDOWS
		return new OS_Windows();
#elif DUSK_LINUX
		return new OS_Linux();
#endif
	}
}
