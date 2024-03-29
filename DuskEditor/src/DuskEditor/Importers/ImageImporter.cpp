#include "ImageImporter.h"

#include "Core/Application/Core.h"
#include "Core/Application/Application.h"
#include "Utils/Compression/Compression.h"

#include "stb/stb_image.h"

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
            headerData.DataMaxSize = width * height * channels;

            void* compressedData = malloc(width * height * channels);

            size_t compressedSize = Compress(compressedData, data, width * height * channels);

            headerData.DataSize = compressedSize;

            size_t dataSize = headerData.DataSize + sizeof(ImageData);
            void* importedFileData = malloc(dataSize);

            memcpy(importedFileData, &headerData, sizeof(headerData));
            memcpy((char*)importedFileData + sizeof(headerData), compressedData, headerData.DataSize);

            std::filesystem::create_directory(Application::Get().GetProjectPath() / ".import");
            std::filesystem::create_directory(Application::Get().GetProjectPath() / ".import/images");

            std::ofstream importFile(importFilePath, std::ios::app | std::ios::binary);
            importFile.write((char*)importedFileData, dataSize);
            importFile.close();

            free(importedFileData);
            free(compressedData);

            TRACE("Imported image {}", imagePath.string().c_str());
        }
        else
        {
            ERR("Error importing image {}", imagePath.string().c_str());

            return;
        }

        stbi_image_free(data);
	}
}
