#include "pch.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace DuskEngine
{
	Ref<Texture> Texture::Create(const std::string& path, const std::string& name)
    {
		return MakeRef<OpenGLTexture>(path, name);
    }

	Ref<Texture> Texture::Create(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		return MakeRef<OpenGLTexture>(path, uuid);
	}
}