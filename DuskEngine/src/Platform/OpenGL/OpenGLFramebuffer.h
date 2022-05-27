#pragma once
#include "Core/Renderer/Resources/Framebuffer.h"

#include "glm/glm.hpp"

namespace DuskEngine
{
	class OpenGLFramebuffer : public FrameBuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		void Bind() const override;
		void Unbind() const override;

		unsigned int GetColorAttachmentID() const override { return m_ColorAttachment; }
		const FramebufferSpecification& GetSpecification() const override { return m_Spec; }

		void Resize();
		void Resize(glm::vec2& viewportSize) override;
	private:
		FramebufferSpecification m_Spec;
		unsigned int m_ID;
		unsigned int m_ColorAttachment, m_DepthAttachment;
	};
}