#include "Framebuffer.h"

#include "GLCommon.h"
#include <iostream>

namespace DuskEngine
{
	Framebuffer::Framebuffer(unsigned int width, unsigned int height)
		:m_ID(0), m_Width(width), m_Height(height), m_Texture(0)
	{
		glGenFramebuffers(1, &m_ID);
		glGenTextures(1, &m_Texture);
		glGenRenderbuffers(1, &m_RenderID);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "error in the framebuffer" << std::endl;
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}