#include "pch.h"
#include "OpenGLTexture.h"

#include "GLCommon.h"

#include "stb/stb_image.h"

#include <filesystem>

namespace DuskEngine
{
    OpenGLTexture::OpenGLTexture(const std::string& filepath, const std::string& name)
        :m_ID(0), m_Width(0), m_Height(0), m_Size(0)
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

    OpenGLTexture::OpenGLTexture(const std::filesystem::path& path, const uuids::uuid& uuid)
        :m_ID(0), m_Width(0), m_Height(0), m_Size(0)
    {
        m_UUID = uuid;
        m_Path = path;
        m_Name = path.filename().string();

        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        unsigned char* data;
        data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

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
            std::string message = "Error loading texture " + m_Name;
            WARN(message.c_str());
            return;
        }

        stbi_image_free(data);

        std::string message = "Created Texture " + m_Name;
        LOG(message.c_str());
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_ID);
        std::string message = "Destroyed Texture " + m_Name;
        LOG(message.c_str());
    }

    void OpenGLTexture::Bind(unsigned int slot) const
    {
        glBindTextureUnit(slot, m_ID);
    }
}