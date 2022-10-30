#include "ImageImporter.h"

#include "Core/Application/Core.h"

#include "stb/stb_image.h"

#include <string>
#include <fstream>

namespace DuskEngine
{
	void ImportImage(const std::filesystem::path& imagePath, const std::filesystem::path& importFilePath)
	{
        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        unsigned char* data;
        data = stbi_load(imagePath.string().c_str(), &width, &height, &channels, 0);

        if (data)
        {
            ImageData headerData;
            headerData.Width = width;
            headerData.Height = height;
            headerData.Channels = channels;
            headerData.DataSize = width * height * channels;

            size_t dataSize = headerData.DataSize + sizeof(ImageData);
            uint8_t* textureData = new uint8_t[dataSize];

            memcpy(textureData, &headerData, sizeof(ImageData));
            memcpy(textureData + sizeof(ImageData), data, headerData.DataSize);

            std::ofstream importFile(importFilePath, std::ios::app | std::ios::binary);
            importFile.write((char*)textureData, dataSize);
            importFile.close();

            delete[] textureData;

            TRACE("Imported image " + imagePath.string());
        }
        else
        {
            ERR("Error importing image " + imagePath.string());

            return;
        }

        stbi_image_free(data);
	}
}
