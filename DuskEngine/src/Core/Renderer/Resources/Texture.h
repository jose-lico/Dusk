#pragma once
#include "../../Macros/DUSK_API.h"

namespace DuskEngine
{
	class DUSK_API Texture
	{
	public:
		static Texture* Create(const char* filepath);

		virtual unsigned int GetRendererID() const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void Bind(unsigned int slot = 0) const = 0;
	};
}