#include "pch.h"
#include "Texture.h"

#include "stb_image.h"
#include "Utils/Logging/Log.h"

namespace DuskEngine
{
    Texture::Texture(const char* filepath, unsigned int type)
        :m_ID(0)
    {
        stbi_set_flip_vertically_on_load(true); // move this somewhere else
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char* data;
        data = stbi_load(filepath, &m_Width, &m_Height, &m_Channels, 3);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, type, m_Width, m_Height, 0, type, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Error loading texture" << std::endl;
        }

        stbi_image_free(data);
    }

    Texture::~Texture()
    {

    }

    void Texture::Bind(unsigned int slot) const
    {
        glBindTextureUnit(slot, m_ID);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}