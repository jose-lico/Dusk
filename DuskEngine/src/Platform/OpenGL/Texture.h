#pragma once
#include "Core/Macros/DUSK_API.h"

namespace DuskEngine
{
	class DUSK_API Texture
	{
	public:
		enum class TextureType
		{
			// from glew.h
			RGB = 0x1907,
			RGBA
		};

		Texture(const char* filepath, TextureType type);
		~Texture();
		void Bind(unsigned int slot) const;
		void Unbind() const;
		unsigned int m_ID;
		int m_Width;
		int m_Height;
	private:
		int m_Channels;
	};
}