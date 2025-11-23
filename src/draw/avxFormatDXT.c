/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#include "avxIcd.h"

#pragma pack(push, 1)

typedef struct dxt1_block {
    afxUnit16 Color0;
    afxUnit16 Color1;
    afxUnit8 Row[4];
} dxt1_block;

typedef struct dxt3_block {
    afxUnit16 AlphaRow[4];
    afxUnit16 Color0;
    afxUnit16 Color1;
    afxUnit8 Row[4];
} dxt3_block;

typedef struct dxt5_block {
    afxUnit8 Alpha[2];
    afxUnit8 AlphaBitmap[6];
    afxUnit16 Color0;
    afxUnit16 Color1;
    afxUnit8 Row[4];
} dxt5_block;

typedef struct texel_block4x4 {
    // row x col
    afxV4d Texel[4][4];
} texel_block4x4;

#pragma pack(pop)

inline void flip_block_s3tc(afxByte* BlockDst, afxByte const* BlockSrc, avxFormat fmt, afxBool HeightTwo)
{
    afxError err = { 0 };
    // There is no distinction between RGB and RGBA in DXT-compressed textures, it is used only to tell OpenGL how to interpret the data.
    // Moreover, in DXT1 (which does not contain an alpha channel), transparency can be emulated using Color0 and Color1 on a per-compression-block basis.
    // There is no difference in how textures with and without transparency are laid out in the file, so they can be flipped using the same method.

    switch (fmt)
    {
    case avxFormat_DXT1un:
    case avxFormat_DXT1v:
    case avxFormat_DXT1A:
    case avxFormat_DXT1Av:
    {
        dxt1_block* Src = (dxt1_block*)BlockSrc;
        dxt1_block* Dst = (dxt1_block*)BlockDst;

        if (HeightTwo)
        {
            Dst->Color0 = Src->Color0;
            Dst->Color1 = Src->Color1;
            Dst->Row[0] = Src->Row[1];
            Dst->Row[1] = Src->Row[0];
            Dst->Row[2] = Src->Row[2];
            Dst->Row[3] = Src->Row[3];
        }
        else
        {
            Dst->Color0 = Src->Color0;
            Dst->Color1 = Src->Color1;
            Dst->Row[0] = Src->Row[3];
            Dst->Row[1] = Src->Row[2];
            Dst->Row[2] = Src->Row[1];
            Dst->Row[3] = Src->Row[0];
        }
        break;
    }
    case avxFormat_DXT3un:
    case avxFormat_DXT3v:
    {
        dxt3_block* Src = (dxt3_block*)BlockSrc;
        dxt3_block* Dst = (dxt3_block*)BlockDst;

        if (HeightTwo)
        {
            Dst->AlphaRow[0] = Src->AlphaRow[1];
            Dst->AlphaRow[1] = Src->AlphaRow[0];
            Dst->AlphaRow[2] = Src->AlphaRow[2];
            Dst->AlphaRow[3] = Src->AlphaRow[3];
            Dst->Color0 = Src->Color0;
            Dst->Color1 = Src->Color1;
            Dst->Row[0] = Src->Row[1];
            Dst->Row[1] = Src->Row[0];
            Dst->Row[2] = Src->Row[2];
            Dst->Row[3] = Src->Row[3];
        }
        else
        {
            Dst->AlphaRow[0] = Src->AlphaRow[3];
            Dst->AlphaRow[1] = Src->AlphaRow[2];
            Dst->AlphaRow[2] = Src->AlphaRow[1];
            Dst->AlphaRow[3] = Src->AlphaRow[0];
            Dst->Color0 = Src->Color0;
            Dst->Color1 = Src->Color1;
            Dst->Row[0] = Src->Row[3];
            Dst->Row[1] = Src->Row[2];
            Dst->Row[2] = Src->Row[1];
            Dst->Row[3] = Src->Row[0];
        }
        break;
    }
    case avxFormat_DXT5un:
    case avxFormat_DXT5v:
    {
        dxt5_block* Src = (dxt5_block*)BlockSrc;
        dxt5_block* Dst = (dxt5_block*)BlockDst;

        if (HeightTwo)
        {
            Dst->Alpha[0] = Src->Alpha[0];
            Dst->Alpha[1] = Src->Alpha[1];
            // the values below are bitmasks used to retrieve alpha values according to the DXT specification
            // 0xF0 == 0b11110000 and 0xF == 0b1111
            Dst->AlphaBitmap[0] = ((Src->AlphaBitmap[1] & 0xF0) >> 4) + ((Src->AlphaBitmap[2] & 0xF) << 4);
            Dst->AlphaBitmap[1] = ((Src->AlphaBitmap[2] & 0xF0) >> 4) + ((Src->AlphaBitmap[0] & 0xF) << 4);
            Dst->AlphaBitmap[2] = ((Src->AlphaBitmap[0] & 0xF0) >> 4) + ((Src->AlphaBitmap[1] & 0xF) << 4);
            Dst->AlphaBitmap[3] = Src->AlphaBitmap[3];
            Dst->AlphaBitmap[4] = Src->AlphaBitmap[4];
            Dst->AlphaBitmap[5] = Src->AlphaBitmap[5];
            Dst->Color0 = Src->Color0;
            Dst->Color1 = Src->Color1;
            Dst->Row[0] = Src->Row[1];
            Dst->Row[1] = Src->Row[0];
            Dst->Row[2] = Src->Row[2];
            Dst->Row[3] = Src->Row[3];
        }
        else
        {
            Dst->Alpha[0] = Src->Alpha[0];
            Dst->Alpha[1] = Src->Alpha[1];
            // the values below are bitmasks used to retrieve alpha values according to the DXT specification
            // 0xF0 == 0b11110000 and 0xF == 0b1111
            Dst->AlphaBitmap[0] = ((Src->AlphaBitmap[4] & 0xF0) >> 4) + ((Src->AlphaBitmap[5] & 0xF) << 4);
            Dst->AlphaBitmap[1] = ((Src->AlphaBitmap[5] & 0xF0) >> 4) + ((Src->AlphaBitmap[3] & 0xF) << 4);
            Dst->AlphaBitmap[2] = ((Src->AlphaBitmap[3] & 0xF0) >> 4) + ((Src->AlphaBitmap[4] & 0xF) << 4);
            Dst->AlphaBitmap[3] = ((Src->AlphaBitmap[1] & 0xF0) >> 4) + ((Src->AlphaBitmap[2] & 0xF) << 4);
            Dst->AlphaBitmap[4] = ((Src->AlphaBitmap[2] & 0xF0) >> 4) + ((Src->AlphaBitmap[0] & 0xF) << 4);
            Dst->AlphaBitmap[5] = ((Src->AlphaBitmap[0] & 0xF0) >> 4) + ((Src->AlphaBitmap[1] & 0xF) << 4);
            Dst->Color0 = Src->Color0;
            Dst->Color1 = Src->Color1;
            Dst->Row[0] = Src->Row[3];
            Dst->Row[1] = Src->Row[2];
            Dst->Row[2] = Src->Row[1];
            Dst->Row[3] = Src->Row[0];
        }
        break;
    }
    default: AfxThrowError();
    }
}

inline void flip_s3tc(afxByte* dst, afxByte const* src, afxUnit srcSiz, afxUnit rowStride, afxUnit rowCnt, avxFormat fmt)
{
    if (rowCnt == 1) AfxCopy(dst, src, srcSiz);
    else
    {
        afxSize XBlocks = rowStride <= 4 ? 1 : rowStride / 4;

        avxFormatDescription pfd;
        AvxDescribeFormats(1, &fmt, &pfd);
        afxUnit blockSiz = pfd.stride;

        if (rowCnt == 2)
        {
            for (afxSize i = 0; i < XBlocks; ++i) // block iterator
                flip_block_s3tc(dst + i * blockSiz, src + i * blockSiz, fmt, TRUE);
        }
        else
        {
            afxSize MaxYBlock = rowCnt / 4 - 1;

            for (afxSize j = 0; j <= MaxYBlock; ++j) // row iterator
                for (afxSize i = 0; i < XBlocks; ++i) // block iterator
                    flip_block_s3tc(dst + (MaxYBlock - j) * blockSiz * XBlocks + i * blockSiz, src + j * blockSiz * XBlocks + i * blockSiz, fmt, FALSE);
        }
    }
}

inline void flip(afxByte* dst, afxByte const* src, afxUnit srcSiz, afxUnit rowStride, afxUnit rowCnt)
{
    for (afxUnit y = 0; y < rowCnt; ++y)
    {
        afxUnit OffsetDst = rowStride * y;
        afxUnit OffsetSrc = srcSiz - (rowStride * (y + 1));
        AfxCopy(dst + OffsetDst, src + OffsetSrc, rowStride);
    }
}

uint16_t rgbTo565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

// Compute color distance squared
uint32_t colorDistanceSquared(uint8_t r0, uint8_t g0, uint8_t b0, uint8_t r1, uint8_t g1, uint8_t b1)
{
    int dr = r0 - r1;
    int dg = g0 - g1;
    int db = b0 - b1;
    return dr * dr + dg * dg + db * db;
}

// Compress a single 4x4 block of RGBA data to DXT1 format
void compressDXT1(const uint8_t* src, uint8_t* dest, afxUnit pixelStride)
{
    uint8_t r[16], g[16], b[16];
    uint8_t minR = 255, minG = 255, minB = 255;
    uint8_t maxR = 0, maxG = 0, maxB = 0;

    // Collect color values and find min/max colors
    for (afxUnit i = 0; i < 16; ++i) {
        r[i] = src[i * pixelStride + 0];
        g[i] = src[i * pixelStride + 1];
        b[i] = src[i * pixelStride + 2];
        if (r[i] < minR) minR = r[i];
        if (g[i] < minG) minG = g[i];
        if (b[i] < minB) minB = b[i];
        if (r[i] > maxR) maxR = r[i];
        if (g[i] > maxG) maxG = g[i];
        if (b[i] > maxB) maxB = b[i];
    }

    // Compute two endpoint colors
    uint16_t color0 = rgbTo565(minR, minG, minB);
    uint16_t color1 = rgbTo565(maxR, maxG, maxB);

    // Pack color endpoints into the destination
    dest[0] = color0 & 0xFF;
    dest[1] = (color0 >> 8) & 0xFF;
    dest[2] = color1 & 0xFF;
    dest[3] = (color1 >> 8) & 0xFF;

    // Compute color index table
    uint8_t indexTable[16];
    for (afxUnit i = 0; i < 16; ++i) {
        uint32_t dist0 = colorDistanceSquared(r[i], g[i], b[i], minR, minG, minB);
        uint32_t dist1 = colorDistanceSquared(r[i], g[i], b[i], maxR, maxG, maxB);
        indexTable[i] = (dist0 < dist1) ? 0 : 1;
    }

    // Pack indices into the remaining 4 bytes of the destination
    dest[4] = (indexTable[0] | (indexTable[1] << 2) | (indexTable[2] << 4) | (indexTable[3] << 6));
    dest[5] = (indexTable[4] | (indexTable[5] << 2) | (indexTable[6] << 4) | (indexTable[7] << 6));
    dest[6] = (indexTable[8] | (indexTable[9] << 2) | (indexTable[10] << 4) | (indexTable[11] << 6));
    dest[7] = (indexTable[12] | (indexTable[13] << 2) | (indexTable[14] << 4) | (indexTable[15] << 6));
}

// Compress a single 4x4 block of RGBA data to DXT3 format
void compressDXT3(const uint8_t* src, uint8_t* dest, afxUnit pixelStride)
{
    uint8_t alpha[16];
    uint8_t r[16], g[16], b[16];
    uint8_t minR = 255, minG = 255, minB = 255;
    uint8_t maxR = 0, maxG = 0, maxB = 0;

    // Collect alpha values and color values
    for (afxUnit i = 0; i < 16; ++i)
    {
        alpha[i] = src[i * pixelStride + 3];
        r[i] = src[i * pixelStride + 0];
        g[i] = src[i * pixelStride + 1];
        b[i] = src[i * pixelStride + 2];
        if (r[i] < minR) minR = r[i];
        if (g[i] < minG) minG = g[i];
        if (b[i] < minB) minB = b[i];
        if (r[i] > maxR) maxR = r[i];
        if (g[i] > maxG) maxG = g[i];
        if (b[i] > maxB) maxB = b[i];
    }

    // Compute two endpoint colors
    uint16_t color0 = rgbTo565(minR, minG, minB);
    uint16_t color1 = rgbTo565(maxR, maxG, maxB);

    // Pack alpha values into the first 8 bytes of the destination
    for (afxUnit i = 0; i < 4; ++i) {
        dest[i * 2 + 0] = alpha[i * 4 + 0];
        dest[i * 2 + 1] = alpha[i * 4 + 1] | (alpha[i * 4 + 2] << 4);
    }

    // Pack color endpoints into the next 8 bytes of the destination
    dest[8] = color0 & 0xFF;
    dest[9] = (color0 >> 8) & 0xFF;
    dest[10] = color1 & 0xFF;
    dest[11] = (color1 >> 8) & 0xFF;

    // Compute color index table
    uint8_t indexTable[16];
    for (afxUnit i = 0; i < 16; ++i) {
        uint32_t dist0 = colorDistanceSquared(r[i], g[i], b[i], minR, minG, minB);
        uint32_t dist1 = colorDistanceSquared(r[i], g[i], b[i], maxR, maxG, maxB);
        indexTable[i] = (dist0 < dist1) ? 0 : 1;
    }

    // Pack indices into the remaining 4 bytes of the destination
    dest[12] = (indexTable[0] | (indexTable[1] << 2) | (indexTable[2] << 4) | (indexTable[3] << 6));
    dest[13] = (indexTable[4] | (indexTable[5] << 2) | (indexTable[6] << 4) | (indexTable[7] << 6));
    dest[14] = (indexTable[8] | (indexTable[9] << 2) | (indexTable[10] << 4) | (indexTable[11] << 6));
    dest[15] = (indexTable[12] | (indexTable[13] << 2) | (indexTable[14] << 4) | (indexTable[15] << 6));
}

// Compress a single 4x4 block of RGBA data to DXT5 format
void compressDXT5(const uint8_t* src, uint8_t* dest, afxUnit pixelStride)
{
    uint8_t alpha[16];
    uint8_t r[16], g[16], b[16];
    uint8_t minR = 255, minG = 255, minB = 255;
    uint8_t maxR = 0, maxG = 0, maxB = 0;

    // Collect alpha values and color values
    for (afxUnit i = 0; i < 16; ++i)
    {
        alpha[i] = src[i * pixelStride + 3];
        r[i] = src[i * pixelStride + 0];
        g[i] = src[i * pixelStride + 1];
        b[i] = src[i * pixelStride + 2];
        if (r[i] < minR) minR = r[i];
        if (g[i] < minG) minG = g[i];
        if (b[i] < minB) minB = b[i];
        if (r[i] > maxR) maxR = r[i];
        if (g[i] > maxG) maxG = g[i];
        if (b[i] > maxB) maxB = b[i];
    }

    // Compute two endpoint colors
    uint16_t color0 = rgbTo565(minR, minG, minB);
    uint16_t color1 = rgbTo565(maxR, maxG, maxB);

    // Compute alpha endpoints
    uint8_t alpha0 = alpha[0];
    uint8_t alpha1 = alpha[0];

    for (afxUnit i = 1; i < 16; ++i)
    {
        if (alpha[i] < alpha0) alpha0 = alpha[i];
        if (alpha[i] > alpha1) alpha1 = alpha[i];
    }

    // Pack alpha values
    uint32_t alphaBlock = 0;
    for (afxUnit i = 0; i < 16; ++i)
    {
        uint8_t alphaVal = alpha[i];
        uint8_t index = 0;
        if (alphaVal > alpha1) {
            index = 0;
        }
        else if (alphaVal > (2 * alpha0 + alpha1) / 3) {
            index = 1;
        }
        else if (alphaVal > (alpha0 + 2 * alpha1) / 3) {
            index = 2;
        }
        else {
            index = 3;
        }
        alphaBlock |= (index << (i * 3));
    }

    dest[0] = alphaBlock & 0xFF;
    dest[1] = (alphaBlock >> 8) & 0xFF;
    dest[2] = (alphaBlock >> 16) & 0xFF;
    dest[3] = (alphaBlock >> 24) & 0xFF;

    // Pack color endpoints
    dest[4] = color0 & 0xFF;
    dest[5] = (color0 >> 8) & 0xFF;
    dest[6] = color1 & 0xFF;
    dest[7] = (color1 >> 8) & 0xFF;

    // Compute color index table
    uint8_t indexTable[16];
    for (afxUnit i = 0; i < 16; ++i) {
        uint32_t dist0 = colorDistanceSquared(r[i], g[i], b[i], minR, minG, minB);
        uint32_t dist1 = colorDistanceSquared(r[i], g[i], b[i], maxR, maxG, maxB);
        indexTable[i] = (dist0 < dist1) ? 0 : 1;
    }

    // Pack indices
    dest[8] = (indexTable[0] | (indexTable[1] << 2) | (indexTable[2] << 4) | (indexTable[3] << 6));
    dest[9] = (indexTable[4] | (indexTable[5] << 2) | (indexTable[6] << 4) | (indexTable[7] << 6));
    dest[10] = (indexTable[8] | (indexTable[9] << 2) | (indexTable[10] << 4) | (indexTable[11] << 6));
    dest[11] = (indexTable[12] | (indexTable[13] << 2) | (indexTable[14] << 4) | (indexTable[15] << 6));
}

#define BLOCK_SIZE 16    // Size of a DXT block in bytes
#define BLOCK_WIDTH 4    // Width of a DXT block in pixels
#define BLOCK_HEIGHT 4   // Height of a DXT block in pixels
#define PIXEL_SIZE 4     // RGBA (4 bytes per pixel)

// Compress a RGBA texel array to DXT1
void compressDXT1Array(afxByte* dst, afxByte const* src, afxUnit srcSiz, afxUnit rowStride, afxUnit rowCnt)
{
    afxUnit width = rowStride;
    afxUnit numBlocksX = (width + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
    afxUnit numBlocksY = (rowCnt + BLOCK_HEIGHT - 1) / BLOCK_HEIGHT;

    for (afxUnit by = 0; by < numBlocksY; ++by)
    {
        for (afxUnit bx = 0; bx < numBlocksX; ++bx)
        {
            uint8_t block[BLOCK_WIDTH * BLOCK_HEIGHT * PIXEL_SIZE];
            uint8_t compressedBlock[BLOCK_SIZE];

            // Gather block data
            for (afxUnit y = 0; y < BLOCK_HEIGHT; ++y)
            {
                for (afxUnit x = 0; x < BLOCK_WIDTH; ++x)
                {
                    afxUnit srcX = bx * BLOCK_WIDTH + x;
                    afxUnit srcY = by * BLOCK_HEIGHT + y;

                    if (srcX < width && srcY < rowCnt)
                    {
                        afxUnit srcIndex = (srcY * width + srcX) * PIXEL_SIZE;
                        afxUnit blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxCopy(block + blockIndex, src + srcIndex, PIXEL_SIZE);
                    }
                    else {
                        // Out of bounds, use transparent black
                        afxUnit blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxZero(block + blockIndex, PIXEL_SIZE);
                    }
                }
            }

            // Compress the block
            compressDXT1(block, compressedBlock, 3);

            // Store compressed block in destination
            afxUnit blockIndex = (by * numBlocksX + bx) * BLOCK_SIZE;
            AfxCopy(dst + blockIndex, compressedBlock, BLOCK_SIZE);
        }
    }
}

// Compress a RGBA texel array to DXT3
void compressDXT3Array(afxByte* dst, afxByte const* src, afxUnit srcSiz, afxUnit rowStride, afxUnit rowCnt)
{
    afxUnit width = rowStride;
    afxUnit numBlocksX = (width + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
    afxUnit numBlocksY = (rowCnt + BLOCK_HEIGHT - 1) / BLOCK_HEIGHT;

    for (afxUnit by = 0; by < numBlocksY; ++by)
    {
        for (afxUnit bx = 0; bx < numBlocksX; ++bx)
        {
            uint8_t block[BLOCK_WIDTH * BLOCK_HEIGHT * PIXEL_SIZE];
            uint8_t compressedBlock[BLOCK_SIZE];

            // Gather block data
            for (afxUnit y = 0; y < BLOCK_HEIGHT; ++y)
            {
                for (afxUnit x = 0; x < BLOCK_WIDTH; ++x)
                {
                    afxUnit srcX = bx * BLOCK_WIDTH + x;
                    afxUnit srcY = by * BLOCK_HEIGHT + y;

                    if (srcX < width && srcY < rowCnt)
                    {
                        afxUnit srcIndex = (srcY * width + srcX) * PIXEL_SIZE;
                        afxUnit blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxCopy(block + blockIndex, src + srcIndex, PIXEL_SIZE);
                    }
                    else {
                        // Out of bounds, use transparent black
                        afxUnit blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxZero(block + blockIndex, PIXEL_SIZE);
                    }
                }
            }

            // Compress the block
            compressDXT3(block, compressedBlock, 4);

            // Store compressed block in destination
            afxUnit blockIndex = (by * numBlocksX + bx) * BLOCK_SIZE;
            AfxCopy(dst + blockIndex, compressedBlock, BLOCK_SIZE);
        }
    }
}

// Compress a RGBA texel array to DXT5
void compressDXT5Array(afxByte* dst, afxByte const* src, afxUnit srcSiz, afxUnit rowStride, afxUnit rowCnt)
{
    afxUnit width = rowStride;
    afxUnit numBlocksX = (width + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
    afxUnit numBlocksY = (rowCnt + BLOCK_HEIGHT - 1) / BLOCK_HEIGHT;

    for (afxUnit by = 0; by < numBlocksY; ++by)
    {
        for (afxUnit bx = 0; bx < numBlocksX; ++bx)
        {
            uint8_t block[BLOCK_WIDTH * BLOCK_HEIGHT * PIXEL_SIZE];
            uint8_t compressedBlock[BLOCK_SIZE];

            // Gather block data
            for (afxUnit y = 0; y < BLOCK_HEIGHT; ++y)
            {
                for (afxUnit x = 0; x < BLOCK_WIDTH; ++x)
                {
                    afxUnit srcX = bx * BLOCK_WIDTH + x;
                    afxUnit srcY = by * BLOCK_HEIGHT + y;

                    if (srcX < width && srcY < rowCnt)
                    {
                        afxUnit srcIndex = (srcY * width + srcX) * PIXEL_SIZE;
                        afxUnit blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxCopy(block + blockIndex, src + srcIndex, PIXEL_SIZE);
                    }
                    else {
                        // Out of bounds, use transparent black
                        afxUnit blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxZero(block + blockIndex, PIXEL_SIZE);
                    }
                }
            }

            // Compress the block
            compressDXT5(block, compressedBlock, 4);

            // Store compressed block in destination
            afxUnit blockIndex = (by * numBlocksX + bx) * BLOCK_SIZE;
            AfxCopy(dst + blockIndex, compressedBlock, BLOCK_SIZE);
        }
    }
}
