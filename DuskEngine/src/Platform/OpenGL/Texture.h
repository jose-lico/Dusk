#pragma once

#include "Core/Assets/Asset.h"

namespace uuids 
{
	class uuid;
}

namespace DuskEngine
{
	struct TextureData
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Channels;
		uint64_t DataSize;
	};

	class Texture : public Asset
	{
	public:
		enum class TextureType
		{
			// from glew.h
			RGB = 0x1907,
			RGBA
		};

		Texture(const std::string& filepath, const std::string& name = "");
		Texture(const std::filesystem::path& path, const uuids::uuid& uuid);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		unsigned int GetRendererID() const { return m_ID; }
		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }
		unsigned int GetSize() const { return m_Height; }

		static void ImportTest(const std::filesystem::path& path, std::ofstream& fout);
	private:
		unsigned int m_ID;
		unsigned int m_Width;
		unsigned int m_Height;
		unsigned int m_Size;
	};
}