#pragma once

#include <uuid.h>
#include <string>
#include <filesystem>

namespace DuskEngine
{
	class Asset
	{
	public:
		Asset() = default;
		Asset(const std::filesystem::path& path, const uuids::uuid& uuid)
			:m_Path(path), m_Name(path.filename().string()), m_UUID(uuid) {}
		~Asset() = default;

		const uuids::uuid& GetUUID() { return m_UUID; }
		const std::string& GetName() { return m_Name; }
		const std::filesystem::path& GetPath() { return m_Path; }
	//protected: // temporary, Asset will be reworked later
		std::filesystem::path m_Path;
		std::string m_Name;
		uuids::uuid m_UUID;
	};
}