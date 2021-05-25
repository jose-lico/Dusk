#pragma once

namespace DuskEngine
{
	class Framebuffer
	{
	public:
		Framebuffer(unsigned int width, unsigned int height);
		~Framebuffer();
		void Bind() const;
		void Unbind() const;
		inline unsigned int GetTexture() const { return m_Texture; }
	private:
		unsigned int m_ID;
		unsigned int m_RenderID;
		unsigned int m_Width;
		unsigned int m_Height;
		unsigned int m_Texture;
	};
}