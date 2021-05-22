#pragma once

namespace DuskEngine
{
	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer();
		void CreateTexture(unsigned int width, unsigned int height);
		void Bind();
		void Unbind();
		unsigned int texture;
	private:
		unsigned int m_ID;
	};
}