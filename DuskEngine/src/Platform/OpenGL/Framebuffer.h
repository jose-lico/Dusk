#pragma once

namespace DuskEngine
{
	struct Framebuffer
	{
		unsigned int ResourceID = 0;
		unsigned int Width, Height;
		unsigned int ColorAttachment, DepthAttachment;
	};
}