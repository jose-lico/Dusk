#include "pch.h"
#include "OpenGLTexture.h"
#include "GLCommon.h"

#include "Core/Macros/LOG.h"

#include <stb_image.h>

namespace DuskEngine
{
    OpenGLTexture::OpenGLTexture(const char* filepath)
        :m_ID(0)
    {
        stbi_set_flip_vertically_on_load(true); // move this somewhere else

        int width, height, channels;
        unsigned char* data;
        data = stbi_load(filepath, &width, &height, &channels, 0);

        LOG(channels)

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
            ERROR("Error loading texture!")
        }

        stbi_image_free(data);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_ID);
    }

    void OpenGLTexture::Bind(unsigned int slot) const
    {
        glBindTextureUnit(slot, m_ID);
    }
}