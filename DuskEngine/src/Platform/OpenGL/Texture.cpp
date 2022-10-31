#include "pch.h"
#include "Texture.h"

#include "OpenGLAPI.h"

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

	Texture CreateTexture(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		Texture textureData;
		textureData.m_UUID = uuid;
		textureData.m_Path = path;
		textureData.m_Name = path.filename().string();

		std::string importFilePath = "res/.import/images/" + path.filename().string() + "-" + uuids::to_string(uuid) + ".import";

		if (std::filesystem::exists(importFilePath))
		{
			std::ifstream importFile(importFilePath, std::ios::binary);

			TextureData headerData;
			importFile.read((char*)&headerData, sizeof(TextureData));

			textureData.Width = headerData.Width;
			textureData.Height = headerData.Height;
			// These values should later be in the meta file
			textureData.Type = TextureType::TEXTURE_2D;
			textureData.WrapMode = WrapMode::CLAMP_TO_EDGE;
			textureData.FilterMode = FilterMode::LINEAR;
			textureData.Mipmaps = true;

			if (headerData.Channels == 4)
				textureData.Format = TextureFormat::RGBA;
			else if (headerData.Channels == 3)
				textureData.Format = TextureFormat::RGB;

			uint8_t* data = new uint8_t[headerData.DataSize];
			importFile.read((char*)data, headerData.DataSize);
			importFile.close();

			OpenGLAPI::SetTextureData(textureData, data);

			delete[] data;
		}
		else
		{
			ERR("Error loading texture " + textureData.m_Name);
			// set pink square or something
			return textureData;
		}

		LOG("Created Texture " + textureData.m_Name);

		return textureData;
	}
}