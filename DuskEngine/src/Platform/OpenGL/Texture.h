#pragma once

#include "Core/Assets/Asset.h"

namespace uuids 
{
	class uuid;
}

namespace DuskEngine
{
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
	private:
		unsigned int m_ID;
		unsigned int m_Width;
		unsigned int m_Height;
		unsigned int m_Size;
	};
}