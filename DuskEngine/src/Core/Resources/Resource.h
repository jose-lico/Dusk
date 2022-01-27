#pragma once

#include <uuid.h>

namespace DuskEngine
{
	class Resource
	{
	public:
		~Resource() = default;

		uuids::uuid GetUUID() { return m_UUID; }
	protected:
		uuids::uuid m_UUID;
	};
}