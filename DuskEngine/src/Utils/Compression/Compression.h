#pragma once

size_t Compress(void* destination, void* source, size_t sourceSize);
size_t Decompress(void* destination, size_t destinationMaxSize, void* source, size_t sourceSize);