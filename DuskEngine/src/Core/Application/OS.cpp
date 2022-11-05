#include "pch.h"
#include "OS.h"

#ifdef DUSK_WINDOWS
#include "Platform/Windows/OS_Windows.h"
#elif DUSK_LINUX
#include "Platform/Linux/OS_Linux.h"
#endif

namespace DuskEngine
{
	OS* OS::m_OS = nullptr;

	OS* OS::Create()
	{
#ifdef DUSK_WINDOWS
		m_OS = new OS_Windows();
		return m_OS;
#elif DUSK_LINUX
		m_OS = new OS_Linux();
		return m_OS;
#endif
	}
}
