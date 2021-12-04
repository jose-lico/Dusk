#include "pch.h"
#include "OpenGLTexture.h"
#include "GLCommon.h"

#include "Core/Macros/LOG.h"

#include <stb_image.h>

namespace DuskEngine
{
    OpenGLTexture::OpenGLTexture(const std::string& filepath, const std::string& name)
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
            ERROR("Error loading texture " + m_Name)
        }

        stbi_image_free(data);

        LOG("Created Texture " + m_Name)
    }

    OpenGLTexture::~OpenGLTexture()
    {
        LOG("Deleting texture " + m_Name)
        glDeleteTextures(1, &m_ID);
    }

    void OpenGLTexture::Bind(unsigned int slot) const
    {
        glBindTextureUnit(slot, m_ID);
    }
}