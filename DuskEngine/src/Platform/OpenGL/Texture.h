#pragma once

#include "Core/Assets/Asset.h"

#include <fstream>

namespace uuids 
{
	class uuid;
}

namespace DuskEngine
{
	enum class TextureType : uint16_t
	{
		TEXTURE_2D = 0x0DE1,
		TEXTURE_3D = 0x806F,
		TEXTURE_CUBE_MAP = 0x8513,
		TEXTURE_2D_ARRAY = 0x8C1A
	};

	enum class TextureFormat : uint16_t
	{
		RGB = 0x1907,
		RGBA = 0x1908
	};

	enum class WrapMode : uint16_t
	{
		REPEAT = 0x2901,
		MIRRORED_REPEAT = 0x8370,
		CLAMP_TO_EDGE = 0x812F,
		CLAMP_TO_BORDER = 0x812D
	};

	enum class FilterMode : uint16_t
	{
		NEAREST = 0x260,
		LINEAR = 0x2601
	};

	struct TextureData
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Channels;
		size_t DataSize;
	};

	struct Texture : public Asset
	{
		uint32_t ResourceID;

		uint32_t Width;
		uint32_t Height;
		uint32_t Channels;
		
		TextureType Type;
		TextureFormat Format;
		WrapMode WrapMode;
		FilterMode FilterMode;
		bool Mipmaps;
	};

	Texture CreateTexture(const unsigned char* data);

	Texture CreateTexture(const std::filesystem::path& path, const uuids::uuid& uuid);
}