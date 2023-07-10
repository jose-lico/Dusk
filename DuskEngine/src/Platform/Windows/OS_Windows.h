#pragma once
#include "Core/Application/OS.h"

namespace DuskEngine
{
	class OS_Windows : public OS
	{
	public:
		OS_Windows();

		virtual const char* GetAppDataFolder() const override { return std::getenv("APPDATA"); }
	};
}