#include "pch.h"
#include "Texture.h"

#include "GLCommon.h"
#include "OpenGLAPI.h"
#include "Utils/Profiling/Timer.h"

#include <filesystem>

namespace DuskEngine
{
    Texture::Texture(const unsigned char* data)
    {
        TextureData headerData;
        memcpy(&headerData, data, sizeof(headerData));

        m_TextureData.Width = headerData.Width;
        m_TextureData.Height = headerData.Height;
        // These values should later be in the meta file
        m_TextureData.Type = TextureType::TEXTURE_2D;
        m_TextureData.WrapMode = WrapMode::CLAMP_TO_EDGE;
        m_TextureData.FilterMode = FilterMode::LINEAR;
        m_TextureData.Mipmaps = true;

        if (headerData.Channels == 4)
            m_TextureData.Format = TextureFormat::RGBA;
        else if (headerData.Channels == 3)
            m_TextureData.Format = TextureFormat::RGB;

        OpenGLAPI::SetTextureData(m_TextureData, data, true);
    }

    Texture::Texture(const std::filesystem::path& path, const uuids::uuid& uuid)
    {
        m_UUID = uuid;
        m_Path = path;
        m_Name = path.filename().string();

        std::string importFilePath = "res/.import/images/" + path.filename().string() + "-" + uuids::to_string(uuid) + ".import";

        if (std::filesystem::exists(importFilePath))
        {
            std::ifstream importFile(importFilePath, std::ios::binary);
            TextureData headerData;

            importFile.read((char*)&headerData, sizeof(TextureData));

            m_TextureData.Width = headerData.Width;
            m_TextureData.Height = headerData.Height;
            // These values should later be in the meta file
            m_TextureData.Type = TextureType::TEXTURE_2D;
            m_TextureData.WrapMode = WrapMode::CLAMP_TO_EDGE;
            m_TextureData.FilterMode = FilterMode::LINEAR;
            m_TextureData.Mipmaps = true;

            if (headerData.Channels == 4)
                m_TextureData.Format = TextureFormat::RGBA;
            else if (headerData.Channels == 3)
                m_TextureData.Format = TextureFormat::RGB;

            uint8_t* textureData = new uint8_t[headerData.DataSize];
            importFile.read((char*)textureData, headerData.DataSize);
            importFile.close();

            OpenGLAPI::SetTextureData(m_TextureData, textureData);
           
            delete[] textureData;
        }
        else
        {
            ERR("Error loading texture " + m_Name);
            return;
        }

        LOG("Created Texture " + m_Name);
    }
}