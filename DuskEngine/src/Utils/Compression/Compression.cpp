#include "pch.h"
#include "Compression.h"

#include "zstd.h"

size_t Compress(void* destination, void* source, size_t sourceSize)
{
    ZSTD_CCtx* cctx = ZSTD_createCCtx();
    ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel, 3);

    size_t compressedMaxSize = ZSTD_compressBound(sourceSize);

    size_t compressedSize = ZSTD_compressCCtx(cctx, destination, compressedMaxSize, source, sourceSize, 3);

    ZSTD_freeCCtx(cctx);

    return compressedSize;
}

size_t Decompress(void* destination, size_t destinationMaxSize, void* source, size_t sourceSize)
{
    ZSTD_DCtx* dctx = ZSTD_createDCtx();

    size_t decompressedSize = ZSTD_decompressDCtx(dctx, destination, destinationMaxSize, source, sourceSize);

    ZSTD_freeDCtx(dctx);

    return decompressedSize;
}
