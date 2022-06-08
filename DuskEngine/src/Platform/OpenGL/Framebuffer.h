#pragma once

#include "glm/glm.hpp"

namespace DuskEngine
{
	struct FramebufferSpecification
	{
		unsigned int Width, Height;
		unsigned int Samples = 1;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetColorAttachmentID() const { return m_ColorAttachment; }
		const FramebufferSpecification& GetSpecification() const { return m_Spec; }

		void Resize();
		void Resize(glm::vec2& viewportSize);
	private:
		FramebufferSpecification m_Spec;

		unsigned int m_ID;
		unsigned int m_ColorAttachment, m_DepthAttachment;
	};
}