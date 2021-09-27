#include "pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace DuskEngine {

	RendererAPI* RenderCommand::s_RendererAPI = RendererAPI::Create();
}