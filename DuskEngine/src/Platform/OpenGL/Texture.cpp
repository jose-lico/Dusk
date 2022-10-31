#include "pch.h"
#include "Texture.h"

#include "GLCommon.h"
#include "Utils/Profiling/Timer.h"

#include "stb/stb_image.h"

#include <filesystem>

namespace DuskEngine
{
    Texture::Texture(const unsigned char* data)
    {
        TextureData headerData;
        memcpy(&headerData, data, sizeof(headerData));

        m_Width = headerData.Width;
        m_Height = headerData.Height;
        
        GLenum type = 0;

        if (headerData.Channels == 4)
            type = GL_RGBA;
        else if (headerData.Channels == 3)
            type = GL_RGB;

        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, type, m_Width, m_Height, 0, type, GL_UNSIGNED_BYTE, data + sizeof(TextureData));

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    Texture::Texture(const std::string& filepath, const std::string& name)
        :m_ID(0), m_Width(0), m_Height(0)
    {
        if (name.empty()) 
        {
            std::string::size_type begin = filepath.rfind("/") + 1;
            std::string::size_type end = filepath.rfind(".");
            std::string filename = filepath.substr(begin, end - begin);
            m_Name = filename;
        }
        else
            m_Name = name;

        stbi_set_flip_vertically_on_load(true); // move this somewhere else

        int width, height, channels;
        unsigned char* data;

        data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (data)
        {
            m_Width = width;
            m_Height = height;
            
            GLenum type = 0;

            if (channels == 4)
            {
                type = GL_RGBA;
            }
            else if (channels == 3)
            {
                type = GL_RGB;
            }

            glGenTextures(1, &m_ID);
            glBindTexture(GL_TEXTURE_2D, m_ID);

            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, type, m_Width, m_Height, 0, type, GL_UNSIGNED_BYTE, data);

            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::string message = "Error loading texture '" + m_Name + "'";
            WARN(message.c_str());
            return;
        }

        stbi_image_free(data);

        std::string message = "Created Texture " + m_Name;
        LOG(message.c_str());
    }

    Texture::Texture(const std::filesystem::path& path, const uuids::uuid& uuid)
        :m_ID(0), m_Width(0), m_Height(0)
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
            m_Width = headerData.Width;
            m_Height = headerData.Height;

            GLenum type = 0;

            if (headerData.Channels == 4)
                type = GL_RGBA;
            else if (headerData.Channels == 3)
                type = GL_RGB;

            uint8_t* textureData = new uint8_t[headerData.DataSize];
            importFile.read((char*)textureData, headerData.DataSize);
                        
            glGenTextures(1, &m_ID);
            glBindTexture(GL_TEXTURE_2D, m_ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, type, m_Width, m_Height, 0, type, GL_UNSIGNED_BYTE, textureData);
            
            glGenerateMipmap(GL_TEXTURE_2D);

            importFile.close();
            delete[] textureData;
        }
        else
        {
            ERR("Error loading texture " + m_Name);
            return;
        }

        LOG("Created Texture " + m_Name);
    }

    Texture::~Texture()
    {
        //glDeleteTextures(1, &m_ID);
        std::string message = "Destroyed Texture " + m_Name;
        LOG(message.c_str());
    }

    void Texture::Bind(unsigned int slot) const
    {
        glBindTextureUnit(slot, m_ID);
    }
}