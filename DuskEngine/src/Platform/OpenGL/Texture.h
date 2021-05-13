#pragma once

#include "GLCommon.h"

namespace DuskEngine
{
	class Texture
	{
	public:
		Texture(const char* filepath, unsigned int type);
		~Texture();
		void Bind(unsigned int slot) const;
		void Unbind() const;
	private:
		unsigned int m_ID;
		int m_Width;
		int m_Height;
		int m_Channels;
	};
}