#pragma once

#include <uuid.h>
#include <string>
#include <filesystem>

namespace DuskEngine
{
	class Resource
	{
	public:
		~Resource() = default;

		const uuids::uuid& GetUUID() { return m_UUID; }
		const std::string& GetName() { return m_Name; }
		const std::filesystem::path& GetPath() { return m_Path; }
	protected:
		uuids::uuid m_UUID;
		std::string m_Name;
		std::filesystem::path m_Path;
	};
}