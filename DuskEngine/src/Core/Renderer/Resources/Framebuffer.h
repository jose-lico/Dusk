#pragma once
#include "Core/Macros/DUSK_API.h"

#include <glm/glm.hpp>

namespace DuskEngine
{
	struct FramebufferSpecification
	{
		unsigned int Width, Height;
		unsigned int Samples = 1;
	};

	class DUSK_EXPORT FrameBuffer
	{
	public:
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual unsigned int GetColorAttachmentID() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(glm::vec2& viewportSize) = 0;

		static FrameBuffer* Create(const FramebufferSpecification& spec);
	};
}