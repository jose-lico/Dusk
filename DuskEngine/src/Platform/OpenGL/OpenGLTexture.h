#pragma once
#include "Core/Renderer/Resources/Texture.h"

namespace DuskEngine
{
	class OpenGLTexture : public Texture
	{
	public:
		enum class TextureType
		{
			// from glew.h
			RGB = 0x1907,
			RGBA
		};

		OpenGLTexture(const std::string& filepath, const std::string& name = "");
		~OpenGLTexture();

		virtual void Bind(unsigned int slot = 0) const override;
		virtual unsigned int GetRendererID() const override { return m_ID; }
		virtual unsigned int GetWidth() const override { return m_Width; }
		virtual unsigned int GetHeight() const override { return m_Height; }
	private:
		unsigned int m_ID;
		unsigned int m_Width;
		unsigned int m_Height;
		std::string m_Name;
	};
}