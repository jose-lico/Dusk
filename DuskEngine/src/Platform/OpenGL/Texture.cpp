#include "pch.h"
#include "Texture.h"

#include "OpenGLAPI.h"
#include "Utils/Compression/Compression.h"

#include <filesystem>

namespace DuskEngine
{
	Texture CreateTexture(const unsigned char* data)
	{
		TextureData headerData;
		memcpy(&headerData, data, sizeof(headerData));

		Texture textureData;
		textureData.Width = headerData.Width;
		textureData.Height = headerData.Height;
		// These values should later be in the meta file
		textureData.Type = TextureType::TEXTURE_2D;
		textureData.WrapMode = WrapMode::CLAMP_TO_EDGE;
		textureData.FilterMode = FilterMode::LINEAR;
		textureData.Mipmaps = false;

		if (headerData.Channels == 4)
			textureData.Format = TextureFormat::RGBA;
		else if (headerData.Channels == 3)
			textureData.Format = TextureFormat::RGB;

		OpenGLAPI::SetTextureData(textureData, data, true);

		return textureData;
	}

	Texture CreateTexture(const std::filesystem::path& path, const uuids::uuid& uuid, const std::string& projectPath)
	{
		Texture texture;
		texture.UUID = uuid;
		texture.Path = path;
		texture.Name = path.filename().string();

		std::filesystem::path importFilePath = (std::filesystem::path)projectPath / (std::filesystem::path)(".import/images/" + path.filename().string() + "-" + uuids::to_string(uuid) + ".import");

		if (std::filesystem::exists(importFilePath))
		{
			std::ifstream importFile(importFilePath, std::ios::binary);
			std::stringstream buffer;
			buffer << importFile.rdbuf();

			TextureData headerData;
			memcpy(&headerData, &buffer.str()[0], sizeof(TextureData));

			texture.Width = headerData.Width;
			texture.Height = headerData.Height;
			// These values should later be in the meta file
			texture.Type = TextureType::TEXTURE_2D;
			texture.WrapMode = WrapMode::CLAMP_TO_EDGE;
			texture.FilterMode = FilterMode::LINEAR;
			texture.Mipmaps = true;

			if (headerData.Channels == 4)
				texture.Format = TextureFormat::SRGBA;
			else if (headerData.Channels == 3)
				texture.Format = TextureFormat::SRGB;

			void* data = malloc(headerData.DataMaxSize);
			Decompress(data, headerData.DataMaxSize, &buffer.str()[0] + sizeof(TextureData), headerData.DataSize);

			OpenGLAPI::SetTextureData(texture, (const unsigned char*)data);

			free(data);
			importFile.close();
		}
		else
		{
			ERR("Error loading texture " + texture.Name);
			// return pink square or something
			return texture;
		}

		LOG("Created Texture " + texture.Name);

		return texture;
	}
}