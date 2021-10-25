#pragma once
#include "Core/Renderer/Resources/Buffer.h"

namespace DuskEngine
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const void* data, unsigned int count);
		~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		unsigned int GetCount() const override { return m_Count; }
	private:
		unsigned int m_ID;
		unsigned int m_Count;
	};
}