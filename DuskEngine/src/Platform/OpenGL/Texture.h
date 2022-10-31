#pragma once

#include "Core/Assets/Asset.h"

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

	struct NewTexture
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

	class Texture : public Asset
	{
	public:
		Texture(const unsigned char* data);
		Texture(const std::filesystem::path& path, const uuids::uuid& uuid);
		~Texture() = default;

		inline const NewTexture& GetTextureData() const { return m_TextureData; }
		//void Bind(unsigned int slot = 0) const;
		//unsigned int GetRendererID() const { return m_ID; }
		//unsigned int GetWidth() const { return m_Width; }
		//unsigned int GetHeight() const { return m_Height; }
		//unsigned int GetSize() const { return m_Height; }
	private:
		NewTexture m_TextureData;
		/*unsigned int m_ID;
		unsigned int m_Width;
		unsigned int m_Height;*/
	};
}