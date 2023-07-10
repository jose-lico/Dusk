#include "pch.h"
#include "OS_Windows.h"

namespace DuskEngine
{
#ifdef DUSK_RELEASE
	void RedirectStream(const char* p_file_name, const char* p_mode, FILE* p_cpp_stream, const DWORD p_std_handle);
#endif

	OS_Windows::OS_Windows()
	{
#ifdef DUSK_RELEASE
		// Yoinked shamelessly from https://github.com/godotengine/godot/blob/master/platform/windows/os_windows.cpp
		if (AttachConsole(ATTACH_PARENT_PROCESS)) 
		{
			RedirectStream("CONIN$", "r", stdin, STD_INPUT_HANDLE);
			RedirectStream("CONOUT$", "w", stdout, STD_OUTPUT_HANDLE);
			RedirectStream("CONOUT$", "w", stderr, STD_ERROR_HANDLE);

			printf("\n"); // Make sure our output is starting from the new line.
		}
			m_AttachedConsole = true;
#elif DUSK_DEBUG
		m_AttachedConsole = true;
#endif
	}

#ifdef DUSK_RELEASE
	// Yoinked shamelessly from https://github.com/godotengine/godot/blob/master/platform/windows/os_windows.cpp
	void RedirectStream(const char* p_file_name, const char* p_mode, FILE* p_cpp_stream, const DWORD p_std_handle) 
	{
		const HANDLE h_existing = GetStdHandle(p_std_handle);
		if (h_existing != INVALID_HANDLE_VALUE) { // Redirect only if attached console have a valid handle.
			const HANDLE h_cpp = reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(p_cpp_stream)));
			if (h_cpp == INVALID_HANDLE_VALUE) { // Redirect only if it's not already redirected to the pipe or file.
				FILE* fp = p_cpp_stream;
				freopen_s(&fp, p_file_name, p_mode, p_cpp_stream); // Redirect stream.
				setvbuf(p_cpp_stream, nullptr, _IONBF, 0); // Disable stream buffering.
			}
		}
	}
#endif
}
