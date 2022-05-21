#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Assets/Asset.h"
#include "Utils/Memory/Memory.h"

#include <string>

namespace DuskEngine
{
	class DUSK_EXPORT Texture : public Asset
	{
	public:
		static Ref<Texture> Create(const std::string& path, const std::string& name = "");
		static Ref<Texture> Create(const std::filesystem::path& path, const uuids::uuid& uuid);

		virtual ~Texture() = default;
		virtual unsigned int GetRendererID() const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void Bind(unsigned int slot = 0) const = 0;

		friend class AssetManager;
		friend class InspectorPanel;
	};
}