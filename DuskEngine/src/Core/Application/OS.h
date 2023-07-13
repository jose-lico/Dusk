#pragma once

namespace DuskEngine
{
	class OS
	{
	public:
		virtual ~OS() = default;

		static OS* Create();

		inline bool IsConsoleAttached() { return m_AttachedConsole; }
		
		virtual std::filesystem::path GetCacheDir() const = 0;
	protected:
		bool m_AttachedConsole = false;
	};
}