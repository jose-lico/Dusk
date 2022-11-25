#pragma once

namespace DuskEngine
{
	class OS
	{
	public:
		virtual ~OS() = default;

		static OS* Create();
		inline static bool IsConsoleAttached() { return m_OS->m_AttachedConsole; }
		virtual char* GetAppDataFolder() const = 0;
	protected:
		bool m_AttachedConsole = false;
	private:
		static OS* m_OS;
	};
}