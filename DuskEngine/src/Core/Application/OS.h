#pragma once

namespace DuskEngine
{
	class OS
	{
	public:
		virtual ~OS() = default;

		static OS* Create();
	};
}