/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#include "../../dev/AvxImplKit.h"

typedef struct _afxTgaImg
{
    afxWhd  whd;
    afxNat  bpp;
    afxByte*data;
} _afxTgaImg;

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(_afxStreamTgaHdr)
{
    afxNat8     idLen; // This field identifies the number of bytes contained in Field 6, the Image ID Field. The maximum number of characters is 255. A value of zero indicates that no Image ID field is included with the image.
    afxNat8     paletteType; // This field indicates the type of color map(if any) included with the image.There are currently 2 defined values for this field: 0 - indicates that no color - map data is included with this image. 1 - indicates that a color - map is included with this image.
    afxNat8     imgType; // The TGA File Format canv be used to store Pseudo-Color, True-Color and Direct-Color images of various pixel depths.Truevision has currently defined seven image types : 0 - No Image Data Included; 1 - Uncompressed, Color - mapped Image; 2 - Uncompressed, True - color Image; 3 - Uncompressed, Black - and-white Image; 9 - Run - length encoded, Color - mapped Image; 10 - Run - length encoded, True - color Image; 11 - Run - length encoded, Black - and-white Image;
    afxNat16    paletteBase; // Index of the first color map entry. Index refers to the starting entry in loading the color map. Example: If you would have 1024 entries in the entire color map but you only need to store 72 of those entries, this field allows you to start in the middle of the color - map(e.g., position 342).
    afxNat16    paletteLen; // Total number of color map entries included.
    afxNat8     paletteBpp; // Establishes the number of bits per entry. Typically 15, 16, 24 or 32-bit values are used.
    afxNat16    originOffX; // These bytes specify the absolute horizontal coordinate for the lower left corner of the image as it is positioned on a display device having an origin at the lower left of the screen(e.g., the TARGA series).
    afxNat16    originOffY; // These bytes specify the absolute vertical coordinate for the lower left corner of the image as it is positioned on a display device having an origin at the lower left of the screen(e.g., the TARGA series).
    afxNat16    width; // This field specifies the width of the image in pixels
    afxNat16    height; // This field specifies the height of the image in pixels.
    afxNat8     bpp; // This field indicates the number of bits per pixel. This number includes the Attribute or Alpha channel bits.Common values are 8, 16, 24 and 32 but other pixel depths could be used
    afxNat8     descriptor; // Bits 3-0: These bits specify the number of attribute bits per pixel. Bits 5 & 4: These bits are used to indicate the order in which pixel data is transferred from the file to the screen. Bit 4 is for left - to - right ordering and bit 5 is for topto - bottom ordering as shown below.
};

AFX_DEFINE_STRUCT(afxTarga)
{
    afxTargaFlags   flags; // 0x01 = Cubemap
    avxFormat       fmt; // raw pixel format or texel block compressed format (aka S3TC/DXT/ASTC)
    afxNat          baseLod;
    afxNat          lodCnt;
    afxWhd          origin;
    afxWhd          whd;
    union
    {
        afxSize     offset; // where start (above this header) the raster data // for each lod, sizeof lod is rowLen * whd[1] * whd[2] * layerCnt 
        void const* src;
        void*       dst;
    }               data;
    afxTargaCodec   codec; // storage compression (not pixel compression), for example, RLE.
    afxNat          encSiz; // the size in bytes of stored image data. If not compressed by any codec, encSize is same as decSize.
    afxNat          decSiz; // the size in bytes of the image subresource. size includes any extra memory that is required based on rowStride.
    afxNat          rowStride; // bytes per row/scanline (W usually aligned/padded to some memory boundary)
    afxNat          depthStride; // bytes per layer or (3D) slice.
    afxNat          udd[2];
};

AFX_DEFINE_STRUCT(afxTargaLayerHdr)
{
    afxSize     offset;
    afxNat      siz;
};

AFX_DEFINE_STRUCT(afxTargaLodHdr)
{
    afxNat32            lodSiz; // whole size of this LOD.
    afxNat32            rowStride;
    afxNat32            zSiz; // sizeof each layer for this LOD
    afxNat32            zStride;
};

AFX_DEFINE_STRUCT(afxTargaFileHdr)
{
    afxNat32            chunkId; // Originally "tga4", a FCC standing for "Targa for Qwadro" file format.
    afxNat32            chunkSiz; // sizeof chunk data
    afxNat32            chunkVer; // ISV-specific FCC related to software used to stream out this file.
    // chunk meta info
    afxNat16            baseLod;
    afxNat16            lodCnt;
    afxNat32            origin[3];
    afxNat32            whd[3];
    avxFormat           fmt;
    afxTargaFlags       flags;
    // chunk data info
    afxUrdOrigin     data; // where start (above this header) the raster data // for each lod, sizeof lod is rowLen * whd[1] * whd[2] * layerCnt 
    afxTargaCodec       codec; // codec used to compress
    afxNat32            encSiz; // compressed size
    afxNat32            decSiz; // uncompressed size
    afxNat32            rowStride; // bytes per row/scanline (W usually aligned/padded to some memory boundary)
    afxNat32            depthStride;
    afxNat32            extraLen;
    // extraLen-long chunk of ISV-extra data (if it has any).
#if 0
    struct
    {
        afxFileReference    data; // where start (above this header) the raster data // for each lod, sizeof lod is rowLen * whd[1] * whd[2] * layerCnt 
        afxNat32            encSiz; // compressed size
        afxNat32            decSiz; // uncompressed size
        afxNat32                rowStride; // bytes per row/scanline (W usually aligned/padded to some memory boundary)
        afxNat32            depthStride;
    } lod[];
#endif
};

typedef struct dxt1_block {
    afxNat16 Color0;
    afxNat16 Color1;
    afxNat8 Row[4];
} dxt1_block;

typedef struct dxt3_block {
    afxNat16 AlphaRow[4];
    afxNat16 Color0;
    afxNat16 Color1;
    afxNat8 Row[4];
} dxt3_block;

typedef struct dxt5_block {
    afxNat8 Alpha[2];
    afxNat8 AlphaBitmap[6];
    afxNat16 Color0;
    afxNat16 Color1;
    afxNat8 Row[4];
} dxt5_block;

typedef struct texel_block4x4 {
    // row x col
    afxV4d Texel[4][4];
} texel_block4x4;

#pragma pack(pop)

inline void flip_block_s3tc(afxByte* BlockDst, afxByte const* BlockSrc, avxFormat fmt, afxBool HeightTwo)
{
    afxError err = NIL;
    // There is no distinction between RGB and RGBA in DXT-compressed textures, it is used only to tell OpenGL how to interpret the data.
    // Moreover, in DXT1 (which does not contain an alpha channel), transparency can be emulated using Color0 and Color1 on a per-compression-block basis.
    // There is no difference in how textures with and without transparency are laid out in the file, so they can be flipped using the same method.

    switch (fmt)
    {
    case avxFormat_DXT1un:
    case avxFormat_DXT1_sRGB:
    case avxFormat_DXT1Aun:
    case avxFormat_DXT1A_sRGB:
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
    case avxFormat_DXT3_sRGB:
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
    case avxFormat_DXT5_sRGB:
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

inline void flip_s3tc(afxByte* dst, afxByte const* src, afxNat srcSiz, afxNat rowStride, afxNat rowCnt, avxFormat fmt)
{
    if (rowCnt == 1) AfxCopy(dst, src, srcSiz);
    else
    {
        afxSize XBlocks = rowStride <= 4 ? 1 : rowStride / 4;

        avxFormatDescription pfd;
        AfxDescribePixelFormat(fmt, &pfd);
        afxNat blockSiz = pfd.stride;

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

inline void flip(afxByte* dst, afxByte const* src, afxNat srcSiz, afxNat rowStride, afxNat rowCnt)
{
    for (afxNat y = 0; y < rowCnt; ++y)
    {
        afxNat OffsetDst = rowStride * y;
        afxNat OffsetSrc = srcSiz - (rowStride * (y + 1));
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
void compressDXT1(const uint8_t* src, uint8_t* dest, afxNat pixelStride)
{
    uint8_t r[16], g[16], b[16];
    uint8_t minR = 255, minG = 255, minB = 255;
    uint8_t maxR = 0, maxG = 0, maxB = 0;

    // Collect color values and find min/max colors
    for (int i = 0; i < 16; ++i) {
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
    for (int i = 0; i < 16; ++i) {
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
void compressDXT3(const uint8_t* src, uint8_t* dest, afxNat pixelStride)
{
    uint8_t alpha[16];
    uint8_t r[16], g[16], b[16];
    uint8_t minR = 255, minG = 255, minB = 255;
    uint8_t maxR = 0, maxG = 0, maxB = 0;

    // Collect alpha values and color values
    for (int i = 0; i < 16; ++i)
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
    for (int i = 0; i < 4; ++i) {
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
    for (int i = 0; i < 16; ++i) {
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
void compressDXT5(const uint8_t* src, uint8_t* dest, afxNat pixelStride)
{
    uint8_t alpha[16];
    uint8_t r[16], g[16], b[16];
    uint8_t minR = 255, minG = 255, minB = 255;
    uint8_t maxR = 0, maxG = 0, maxB = 0;

    // Collect alpha values and color values
    for (int i = 0; i < 16; ++i)
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

    for (int i = 1; i < 16; ++i)
    {
        if (alpha[i] < alpha0) alpha0 = alpha[i];
        if (alpha[i] > alpha1) alpha1 = alpha[i];
    }

    // Pack alpha values
    uint32_t alphaBlock = 0;
    for (int i = 0; i < 16; ++i)
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
    for (int i = 0; i < 16; ++i) {
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
void compressDXT1Array(afxByte* dst, afxByte const* src, afxNat srcSiz, afxNat rowStride, afxNat rowCnt)
{
    afxNat width = rowStride;
    int numBlocksX = (width + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
    int numBlocksY = (rowCnt + BLOCK_HEIGHT - 1) / BLOCK_HEIGHT;

    for (int by = 0; by < numBlocksY; ++by)
    {
        for (int bx = 0; bx < numBlocksX; ++bx)
        {
            uint8_t block[BLOCK_WIDTH * BLOCK_HEIGHT * PIXEL_SIZE];
            uint8_t compressedBlock[BLOCK_SIZE];

            // Gather block data
            for (int y = 0; y < BLOCK_HEIGHT; ++y)
            {
                for (int x = 0; x < BLOCK_WIDTH; ++x)
                {
                    int srcX = bx * BLOCK_WIDTH + x;
                    int srcY = by * BLOCK_HEIGHT + y;

                    if (srcX < width && srcY < rowCnt)
                    {
                        int srcIndex = (srcY * width + srcX) * PIXEL_SIZE;
                        int blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxCopy(block + blockIndex, src + srcIndex, PIXEL_SIZE);
                    }
                    else {
                        // Out of bounds, use transparent black
                        int blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxZero(block + blockIndex, PIXEL_SIZE);
                    }
                }
            }

            // Compress the block
            compressDXT1(block, compressedBlock, 3);

            // Store compressed block in destination
            int blockIndex = (by * numBlocksX + bx) * BLOCK_SIZE;
            AfxCopy(dst + blockIndex, compressedBlock, BLOCK_SIZE);
        }
    }
}

// Compress a RGBA texel array to DXT3
void compressDXT3Array(afxByte* dst, afxByte const* src, afxNat srcSiz, afxNat rowStride, afxNat rowCnt)
{
    afxNat width = rowStride;
    int numBlocksX = (width + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
    int numBlocksY = (rowCnt + BLOCK_HEIGHT - 1) / BLOCK_HEIGHT;

    for (int by = 0; by < numBlocksY; ++by)
    {
        for (int bx = 0; bx < numBlocksX; ++bx)
        {
            uint8_t block[BLOCK_WIDTH * BLOCK_HEIGHT * PIXEL_SIZE];
            uint8_t compressedBlock[BLOCK_SIZE];

            // Gather block data
            for (int y = 0; y < BLOCK_HEIGHT; ++y)
            {
                for (int x = 0; x < BLOCK_WIDTH; ++x)
                {
                    int srcX = bx * BLOCK_WIDTH + x;
                    int srcY = by * BLOCK_HEIGHT + y;

                    if (srcX < width && srcY < rowCnt)
                    {
                        int srcIndex = (srcY * width + srcX) * PIXEL_SIZE;
                        int blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxCopy(block + blockIndex, src + srcIndex, PIXEL_SIZE);
                    }
                    else {
                        // Out of bounds, use transparent black
                        int blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxZero(block + blockIndex, PIXEL_SIZE);
                    }
                }
            }

            // Compress the block
            compressDXT3(block, compressedBlock, 4);

            // Store compressed block in destination
            int blockIndex = (by * numBlocksX + bx) * BLOCK_SIZE;
            AfxCopy(dst + blockIndex, compressedBlock, BLOCK_SIZE);
        }
    }
}

// Compress a RGBA texel array to DXT5
void compressDXT5Array(afxByte* dst, afxByte const* src, afxNat srcSiz, afxNat rowStride, afxNat rowCnt)
{
    afxNat width = rowStride;
    int numBlocksX = (width + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
    int numBlocksY = (rowCnt + BLOCK_HEIGHT - 1) / BLOCK_HEIGHT;

    for (int by = 0; by < numBlocksY; ++by)
    {
        for (int bx = 0; bx < numBlocksX; ++bx)
        {
            uint8_t block[BLOCK_WIDTH * BLOCK_HEIGHT * PIXEL_SIZE];
            uint8_t compressedBlock[BLOCK_SIZE];

            // Gather block data
            for (int y = 0; y < BLOCK_HEIGHT; ++y)
            {
                for (int x = 0; x < BLOCK_WIDTH; ++x)
                {
                    int srcX = bx * BLOCK_WIDTH + x;
                    int srcY = by * BLOCK_HEIGHT + y;

                    if (srcX < width && srcY < rowCnt)
                    {
                        int srcIndex = (srcY * width + srcX) * PIXEL_SIZE;
                        int blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxCopy(block + blockIndex, src + srcIndex, PIXEL_SIZE);
                    }
                    else {
                        // Out of bounds, use transparent black
                        int blockIndex = (y * BLOCK_WIDTH + x) * PIXEL_SIZE;
                        AfxZero(block + blockIndex, PIXEL_SIZE);
                    }
                }
            }

            // Compress the block
            compressDXT5(block, compressedBlock, 4);

            // Store compressed block in destination
            int blockIndex = (by * numBlocksX + bx) * BLOCK_SIZE;
            AfxCopy(dst + blockIndex, compressedBlock, BLOCK_SIZE);
        }
    }
}







void DecompressRleChunk(afxStream stream, afxNat width, afxNat height, afxNat bpp, afxByte *dst)
{
    afxByte byteCnt = bpp >> 3;
    afxNat siz = width * height * byteCnt;
    afxNat currByte = 0;

    while (currByte < siz)
    {
        afxByte hdr;
        AfxReadStream(stream, sizeof(afxByte) * 1, 0, &hdr);

        afxNat i, runLen = (hdr & 0x7F) + 1;

        if (hdr & 0x80)
        {
            afxByte buf[4];
            AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, buf);

            for (i = 0; i < runLen; i++)
            {
                AfxCopy2(&dst[currByte], buf, sizeof(afxByte), byteCnt);
                currByte += byteCnt;
            }
        }
        else
        {
            for (i = 0; i < runLen; i++)
            {
                AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, &dst[currByte]);
                currByte += byteCnt;
            }
        }
    }
}

void DecodeRle8Chunk(afxStream stream, afxNat siz, afxNat len, afxByte *dst)
{
    afxNat currByte = 0;

    while (currByte < len)
    {
        afxByte hdr;
        AfxReadStream(stream, sizeof(afxByte) * 1, 0, &hdr);

        afxNat i, runLen = (hdr & 0x7F) + 1;

        if (hdr & 0x80)
        {
            afxByte buf[4];
            AfxReadStream(stream, sizeof(afxByte) * siz, 0, buf);

            for (i = 0; i < runLen; i++)
            {
                AfxCopy2(&dst[currByte], buf, sizeof(afxByte), siz);
                currByte += siz;
            }
        }
        else
        {
            for (i = 0; i < runLen; i++)
            {
                AfxReadStream(stream, sizeof(afxByte) * siz, 0, &dst[currByte]);
                currByte += siz;
            }
        }
    }
}

_AVX afxBool AfxSetUpTarga(afxTarga* tga, afxRaster ras, afxRasterIo const* op, afxNat lodCnt)
{
    afxError err = NIL;
    *tga = (afxTarga) { 0 };
    
    if (ras)
        tga->fmt = AfxGetRasterFormat(ras);

    if (op)
    {
        tga->baseLod = op->rgn.lodIdx;
        tga->lodCnt = AfxMax(1, lodCnt);
        tga->origin[0] = op->rgn.origin[0];
        tga->origin[1] = op->rgn.origin[1];
        tga->origin[2] = op->rgn.origin[2];
        tga->whd[0] = AfxMax(1, op->rgn.whd[0]);
        tga->whd[1] = AfxMax(1, op->rgn.whd[1]);
        tga->whd[2] = AfxMax(1, op->rgn.whd[2]);

        tga->rowStride = op->rowStride;
    }
    else if (ras)
    {
        AfxGetRasterExtent(ras, 0, tga->whd);
        afxNat maxLodCnt = AfxCountRasterMipmaps(ras);

        tga->lodCnt = AfxMin(AfxMax(1, lodCnt), maxLodCnt);

        if (AfxTestRasterFlags(ras, afxRasterFlag_CUBEMAP))
            tga->flags |= afxTargaFlag_CUBEMAP;

        if (AfxTestRasterFlags(ras, afxRasterFlag_3D))
            tga->flags |= afxTargaFlag_3D;
    }
    else AfxThrowError();

    afxRasterLayout layout;
    AfxQueryRasterLayout(ras, 0, 0, &layout);
    tga->rowStride = tga->rowStride ? tga->rowStride : layout.rowStride;
    tga->depthStride = tga->depthStride ? tga->depthStride : layout.depthStride;

    return err;
}

_AVX afxBool AfxReadTarga(afxTarga* tga, afxStream in)
{
    afxError err = NIL;
    afxTargaFileHdr hdr3;
    afxNat bkpOff = AfxAskStreamPosn(in);
    AfxReadStream(in, sizeof(hdr3), 0, &hdr3);

    if (hdr3.chunkId == AFX_MAKE_FCC('t', 'g', 'a', '4')) // Try read as the SIGMA-engineered Targa format.
    {
        tga->lodCnt = hdr3.lodCnt;
        tga->origin[0] = hdr3.origin[0];
        tga->origin[1] = hdr3.origin[1];
        tga->origin[2] = hdr3.origin[2];
        tga->whd[0] = hdr3.whd[0];
        tga->whd[1] = hdr3.whd[1];
        tga->whd[2] = hdr3.whd[2];

        tga->fmt = hdr3.fmt;
        tga->codec = hdr3.codec;
        tga->decSiz = hdr3.decSiz;
        tga->encSiz = hdr3.encSiz;
        tga->flags = hdr3.flags;
        tga->depthStride = hdr3.depthStride;
        tga->rowStride = hdr3.rowStride;
        tga->data.offset = hdr3.data.offset;
    }
    else // Try read as the Truevision standard TGA format.
    {
        AfxSeekStream(in, bkpOff, afxSeekOrigin_BEGIN);
        _afxStreamTgaHdr hdr2;

        if (AfxReadStream(in, sizeof(hdr2), 0, &hdr2)) AfxThrowError();
        else
        {
            tga->data.offset = AfxAskStreamPosn(in);

            tga->udd[0] = NIL;
            tga->udd[1] = NIL;
            tga->flags = NIL;

            tga->lodCnt = 1;
            tga->origin[0] = hdr2.originOffX;
            tga->origin[1] = hdr2.originOffY;
            tga->origin[2] = 0;            
            tga->whd[0] = hdr2.width;
            tga->whd[1] = hdr2.height;
            tga->whd[2] = 1;
            tga->depthStride = 1;

            switch (hdr2.bpp)
            {
            case 32:
                tga->rowStride = tga->whd[0] * 4;
                tga->decSiz = tga->rowStride * tga->whd[1];
                tga->fmt = avxFormat_BGRA8;

                switch (hdr2.imgType)
                {
                case 2: // Uncompressed, RGB images.
                    tga->codec = afxTargaCodec_NONE;
                    tga->encSiz = tga->decSiz;
                    break;
                case 10: // Runlength encoded RGB images.
                    tga->codec = afxTargaCodec_RLE8;
                    tga->encSiz = tga->whd[0] * tga->whd[1] * (hdr2.bpp >> 3);
                    break;
                default: AfxThrowError(); break;
                }
                break;
            case 24:
                tga->rowStride = tga->whd[0] * 3;
                tga->decSiz = tga->rowStride * tga->whd[1];
                tga->fmt = avxFormat_BGR8;

                switch (hdr2.imgType)
                {
                case 2: // Uncompressed, RGB images.
                    tga->codec = afxTargaCodec_NONE;
                    tga->encSiz = tga->decSiz;
                    break;
                case 10: // Runlength encoded RGB images.
                    tga->codec = afxTargaCodec_RLE8;
                    tga->encSiz = tga->whd[0] * tga->whd[1] * (hdr2.bpp >> 3);
                    break;
                default: AfxThrowError(); break;
                }
                break;
            case 16:
                tga->rowStride = tga->whd[0] * 2;
                tga->decSiz = tga->rowStride * tga->whd[1];
                tga->fmt = avxFormat_RGBA4;

                switch (hdr2.imgType)
                {
                case 2: // Uncompressed, RGB images.
                    tga->codec = afxTargaCodec_NONE;
                    tga->encSiz = tga->decSiz;
                    break;
                case 10: // Runlength encoded RGB images.
                    tga->codec = afxTargaCodec_RLE8;
                    tga->encSiz = tga->whd[0] * tga->whd[1] * (hdr2.bpp >> 3);
                    break;
                default: AfxThrowError(); break;
                }
                break;
            case 15:
                tga->rowStride = tga->whd[0] * 2;
                tga->decSiz = tga->rowStride * tga->whd[1];
                tga->fmt = hdr2.descriptor & 0x3 ? avxFormat_RGB5A1 : avxFormat_R5G6B5;

                switch (hdr2.imgType)
                {
                case 2: // Uncompressed, RGB images.
                    tga->codec = afxTargaCodec_NONE;
                    tga->encSiz = tga->decSiz;
                    break;
                case 10: // Runlength encoded RGB images.
                    tga->codec = afxTargaCodec_RLE8;
                    tga->encSiz = tga->whd[0] * tga->whd[1] * (hdr2.bpp >> 3);
                    break;
                default: AfxThrowError(); break;
                }
                break;
            case  8:
                if (!hdr2.paletteType)
                {
                    tga->rowStride = tga->whd[0] * 1;
                    tga->decSiz = tga->rowStride * tga->whd[1];
                    tga->fmt = avxFormat_R8;

                    switch (hdr2.imgType)
                    {
                    case 2: // Uncompressed, RGB images.
                        tga->codec = afxTargaCodec_NONE;
                        tga->encSiz = tga->decSiz;
                        break;
                    case 3: // Uncompressed, black and white images.
                        // set pixel format for Intensity?
                        tga->codec = afxTargaCodec_NONE;
                        tga->encSiz = tga->decSiz;
                        break;
                    case 10: // Runlength encoded RGB images.
                        tga->codec = afxTargaCodec_RLE8;
                        tga->encSiz = tga->whd[0] * tga->whd[1] * (hdr2.bpp >> 3);
                        break;
                    case 11: // Compressed, black and white images.
                        tga->codec = afxTargaCodec_RLE8;
                        tga->encSiz = tga->whd[0] * tga->whd[1] * (hdr2.bpp >> 3);
                        break;
                    default: AfxThrowError(); break;
                    }

                }
                else // indexed
                {
                    switch (hdr2.paletteBpp)
                    {
                    case 32:
                        tga->rowStride = tga->whd[0] * 4;
                        tga->decSiz = tga->rowStride * tga->whd[1];
                        tga->fmt = avxFormat_BGRA8;

                        switch (hdr2.imgType)
                        {
                        case 1: // Uncompressed, color-mapped images.
                            tga->codec = afxTargaCodec_NONE;
                            tga->encSiz = tga->decSiz;
                            break;
                        case 9: // Runlength encoded color-mapped images.
                            tga->codec = afxTargaCodec_RLE8;
                            tga->encSiz = tga->decSiz;
                            break;
                        default: AfxThrowError(); break;
                        }
                        break;
                    case 24:
                        tga->rowStride = tga->whd[0] * 3;
                        tga->decSiz = tga->rowStride * tga->whd[1];
                        tga->fmt = avxFormat_BGR8;

                        switch (hdr2.imgType)
                        {
                        case 1: // Uncompressed, color-mapped images.
                            tga->codec = afxTargaCodec_NONE;
                            tga->encSiz = tga->decSiz;
                            break;
                        case 9: // Runlength encoded color-mapped images.
                            tga->codec = afxTargaCodec_RLE8;
                            tga->encSiz = tga->decSiz;
                            break;
                        default: AfxThrowError(); break;
                        }
                        break;
                    case 16:
                        tga->rowStride = tga->whd[0] * 2;
                        tga->decSiz = tga->rowStride * tga->whd[1];
                        tga->fmt = avxFormat_RGBA4;

                        switch (hdr2.imgType)
                        {
                        case 1: // Uncompressed, color-mapped images.
                            tga->codec = afxTargaCodec_NONE;
                            tga->encSiz = tga->decSiz;
                            break;
                        case 9: // Runlength encoded color-mapped images.
                            tga->codec = afxTargaCodec_RLE8;
                            tga->encSiz = tga->decSiz;
                            break;
                        default: AfxThrowError(); break;
                        }
                        break;
                    case 15:
                        tga->rowStride = tga->whd[0] * 2;
                        tga->decSiz = tga->rowStride * tga->whd[1];
                        tga->fmt = hdr2.descriptor & 0x3 ? avxFormat_RGB5A1 : avxFormat_R5G6B5;

                        switch (hdr2.imgType)
                        {
                        case 1: // Uncompressed, color-mapped images.
                            tga->codec = afxTargaCodec_NONE;
                            tga->encSiz = tga->decSiz;
                            break;
                        case 9: // Runlength encoded color-mapped images.
                            tga->codec = afxTargaCodec_RLE8;
                            tga->encSiz = tga->decSiz;
                            break;
                        default: AfxThrowError(); break;
                        }
                        break;
                    case 8:
                        tga->rowStride = tga->whd[0] * 1;
                        tga->decSiz = tga->rowStride * tga->whd[1];
                        tga->fmt = avxFormat_R8;

                        switch (hdr2.imgType)
                        {
                        case 1: // Uncompressed, color-mapped images.
                            tga->codec = afxTargaCodec_NONE;
                            tga->encSiz = tga->decSiz;
                            break;
                        case 9: // Runlength encoded color-mapped images.
                            tga->codec = afxTargaCodec_RLE8;
                            tga->encSiz = tga->decSiz;
                            break;
                        default: AfxThrowError(); break;
                        }
                        break;
                    default: AfxThrowError(); break;
                    }
                }
                break;
            }

            if (!err)
            {

            }
        }
    }
    return !err;
}

_AVX afxBool AfxDoTargaInput(afxTarga* tga, afxStream in, void* dst)
{
    afxError err = NIL;
    
    switch (tga->codec)
    {
    case afxTargaCodec_NONE:
    {
        if (!err)
        {
            if (AfxReadStream(in, tga->rowStride * tga->whd[1] * tga->whd[2], 0, dst))
                AfxThrowError();
        }
        break;
    }
    case afxTargaCodec_RLE8:
    {
        afxNat pixelStride = tga->rowStride / tga->whd[0];

        if (!err)
        {
            DecodeRle8Chunk(in, pixelStride, tga->whd[0] * tga->whd[1] * tga->whd[2], dst);
        }
        break;
    }
    case afxTargaCodec_GDEFLATE:
    {
        AfxThrowError();
        break;
    }
    default: AfxThrowError();
    }
    return !err;
}

_AVX afxBool AfxBeginTargaOutput(afxTarga* tga, afxStream out)
{
    afxError err = NIL;
    _afxStreamTgaHdr hdr2;

    switch (tga->fmt)
    {
    case avxFormat_R8:
    case avxFormat_S8:
        hdr2.bpp = 8; break;
    case avxFormat_RG8:
    case avxFormat_D16:
    case avxFormat_RGBA4:
    case avxFormat_RGB5A1:
    case avxFormat_R5G6B5:
        hdr2.bpp = 16; break;
    case avxFormat_RGB8:
    case avxFormat_BGR8:
    case avxFormat_RGB8_sRGB:
        hdr2.bpp = 24; break;
    case avxFormat_RGBA8:
    case avxFormat_BGRA8:
    case avxFormat_RGBA8_sRGB:
    case avxFormat_R32f:
    case avxFormat_E5BGR9uf:
    case avxFormat_A2RGB10:
    case avxFormat_D32f:
    case avxFormat_D24S8:
    case avxFormat_X8D24:
        hdr2.bpp = 32; break;
    default: AfxThrowError(); break;
    }

    hdr2.idLen = 0;
    hdr2.paletteType = 0;
    hdr2.imgType = hdr2.bpp == 8 ? 3 : 2;
    hdr2.paletteBase = 0;
    hdr2.paletteLen = 0;
    hdr2.paletteBpp = 0;
    hdr2.originOffX = tga->origin[0];
    hdr2.originOffY = tga->origin[1];
    hdr2.width = tga->whd[0];
    hdr2.height = tga->whd[1];
    hdr2.bpp = hdr2.bpp;
    hdr2.descriptor = 0;

    if (AfxWriteStream(out, sizeof(hdr2), 0, &hdr2))
        AfxThrowError();

    return !err;
}

_AVX afxBool AfxEndTargaOutput(afxTarga* tga, afxStream out)
{
    afxError err = NIL;
    afxTargaFileHdr hdr3 = { 0 };
    hdr3.codec = tga->codec;
    hdr3.decSiz = tga->decSiz;
    hdr3.encSiz = tga->encSiz;
    hdr3.depthStride = tga->depthStride;
    hdr3.chunkId = AFX_MAKE_FCC('t', 'g', 'a', '4');
    hdr3.flags = tga->flags;
    hdr3.fmt = tga->fmt;
    hdr3.lodCnt = tga->lodCnt;
    hdr3.origin[0] = tga->origin[0];
    hdr3.origin[1] = tga->origin[1];
    hdr3.origin[2] = tga->origin[2];
    hdr3.whd[0] = tga->whd[0];
    hdr3.whd[1] = tga->whd[1];
    hdr3.whd[2] = tga->whd[2];
    hdr3.rowStride = tga->rowStride;
    hdr3.data.offset = tga->data.offset;

    if (AfxWriteStream(out, sizeof(hdr3), 0, &hdr3))
        AfxThrowError();

    return !err;
}

_AVX afxBool AfxDoTargaOutput(afxTarga* tga, afxNat iterNo, void const* src, afxStream out)
{
    afxError err = NIL;

    if (iterNo == 0)
    {
        AfxBeginTargaOutput(tga, out);
        
        if (AfxWriteStream(out, tga->rowStride * tga->whd[1] * tga->depthStride, 0, src))
            AfxThrowError();
    }

    AfxThrowError();
    return err;
}

_AVX afxBool TGA_Load(afxStream in, _afxTgaImg* img)
{
    afxError err = NIL;    
    int i;

    _afxStreamTgaHdr hdr;
    AfxReadStream(in, sizeof(hdr), 0, &hdr);
    //AfxAdvanceStream(stream, hdr.idLen);

    switch (hdr.imgType)
    {
    case 1:
    case 2:
    case 3:
    case 9:
    case 10:
    case 11: break;
    default: AfxThrowError();
    }

    if (!err)
    {
        switch (hdr.bpp)
        {
        case 32:
        {
            if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat32), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
            else
            {
                if (hdr.imgType == 10)
                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, img->data);
                else
                    if (AfxReadStream(in, hdr.width * hdr.height * sizeof(afxNat32), 0, img->data))
                        AfxThrowError();
            }
            break;
        }
        case 24:
        {
            if (!(img->data = AfxAllocate(hdr.width * hdr.height, 3 * sizeof(afxByte), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
            else
            {
                if (hdr.imgType == 10)
                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, img->data);
                else
                    if (AfxReadStream(in, hdr.width * hdr.height * 3, 0, img->data))
                        AfxThrowError();
            }
            break;
        }
        case 16:
        case 15:
        {
            if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat16), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
            else
            {
                if (hdr.imgType == 10)
                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, img->data);
                else
                    if (AfxReadStream(in, sizeof(afxNat16) * hdr.width * hdr.height, 0, img->data))
                        AfxThrowError();

                hdr.bpp = 16;
            }
            break;
        }
        case 8:
        {
            if (hdr.paletteType)
            {
                afxByte *indices = NIL;
                afxByte *palette = NIL;

                switch (hdr.paletteBpp)
                {
                case 32:
                {
                    if (!(palette = AfxAllocate(hdr.paletteLen, sizeof(afxNat32), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                    else
                    {
                        if (AfxReadStream(in, hdr.paletteLen * sizeof(afxNat32), 0, palette)) AfxThrowError();
                        else
                        {
                            if (!(indices = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, indices);
                                else
                                    if (AfxReadStream(in, hdr.width * hdr.height, 0, indices))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat32), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                                else
                                {
                                    for (i = 0; i < hdr.width * hdr.height; i++)
                                    {
                                        img->data[4 * i + 0] = palette[4 * indices[i] + 0];
                                        img->data[4 * i + 1] = palette[4 * indices[i] + 1];
                                        img->data[4 * i + 2] = palette[4 * indices[i] + 2];
                                        img->data[4 * i + 3] = palette[4 * indices[i] + 3]; // <
                                    }
                                    hdr.bpp = 32;
                                }
                            }
                        }
                    }
                    break;
                }
                case 24:
                {
                    if (!(palette = AfxAllocate(hdr.paletteLen, 3 * sizeof(afxByte), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                    else
                    {
                        if (AfxReadStream(in, hdr.paletteLen * 3, 0, palette)) AfxThrowError();
                        else
                        {
                            if (!(indices = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, indices);
                                else
                                    if (AfxReadStream(in, hdr.width * hdr.height, 0, indices))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, 3 * sizeof(afxByte), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                                else
                                {
                                    for (i = 0; i < hdr.width * hdr.height; i++)
                                    {
                                        img->data[3 * i + 0] = palette[3 * indices[i] + 0];
                                        img->data[3 * i + 1] = palette[3 * indices[i] + 1];
                                        img->data[3 * i + 2] = palette[3 * indices[i] + 2];
                                    }
                                    hdr.bpp = 24;
                                }
                            }
                        }
                    }
                    break;
                }
                case 16:
                case 15:
                {
                    if (!(palette = AfxAllocate(hdr.paletteLen, sizeof(afxNat16), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                    else
                    {
                        if (AfxReadStream(in, hdr.paletteLen * sizeof(afxNat16), 0, palette)) AfxThrowError();
                        else
                        {
                            if (!(indices = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, indices);
                                else
                                    if (AfxReadStream(in, hdr.width * hdr.height, 0, indices))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat16), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                                else
                                {
                                    for (i = 0; i < hdr.width * hdr.height; i++)
                                        ((afxNat16*)img->data)[i] = ((afxNat16*)palette)[indices[i]];

                                    hdr.bpp = 16;
                                }
                            }
                        }
                    }
                    break;
                }
                default: AfxThrowError(); break;
                }
                
                if (indices)
                    AfxDeallocate(indices);

                if (palette)
                    AfxDeallocate(palette);
            }
            else
            {
                if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat8), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                else
                {
                    if (hdr.imgType == 11)
                        DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, img->data);
                    else
                        if (AfxReadStream(in, hdr.width * hdr.height * sizeof(afxNat8), 0, img->data))
                            AfxThrowError();
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }

        if (!err)
        {
#if 0
            /*
                These bits are used to indicate the order in which pixel data is transferred from the file to the screen.
                Bit 4 (0x16) is for left-to-right ordering and bit 5 (0x32) is for top-to-bottom ordering as shown below.
            */
            if (!(hdr.descriptor & 0x20))
            {
                afxNat scanline = hdr.width * (hdr.bpp >> 3);
                afxNat siz = scanline * hdr.height;
                afxByte *buf;

                if (!(buf = AfxAllocate(1, siz, AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                else
                {
                    for (i = 0; i < hdr.height; i++)
                        AfxCopy2(1, scanline, img->data + i * scanline, buf + (siz - (i + 1) * scanline));

                    AfxCopy2(1, siz, buf, img->data);

                    AfxDeallocate(buf);
                }
            }
#endif

            img->whd[0] = hdr.width;
            img->whd[1] = hdr.height;
            img->whd[2] = 1;
            img->bpp = hdr.bpp;
        }
    }
    return err;
}

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(_afxSerializedTargaHdr)
{
    afxNat8 idLength; // This field identifies the number of bytes contained in Field 6, the Image ID Field. The maximum number of characters is 255. A value of zero indicates that no Image ID field is included with the image.
    afxInt8 colorMapType; // This field indicates the type of color map(if any) included with the image.There are currently 2 defined values for this field: 0 - indicates that no color - map data is included with this image. 1 - indicates that a color - map is included with this image.
    afxInt8 imageType; // The TGA File Format canv be used to store Pseudo-Color, True-Color and Direct-Color images of various pixel depths.Truevision has currently defined seven image types : 0 - No Image Data Included; 1 - Uncompressed, Color - mapped Image; 2 - Uncompressed, True - color Image; 3 - Uncompressed, Black - and-white Image; 9 - Run - length encoded, Color - mapped Image; 10 - Run - length encoded, True - color Image; 11 - Run - length encoded, Black - and-white Image;
    struct
    {
        afxNat16 firstEntryIdx; // Index of the first color map entry. Index refers to the starting entry in loading the color map. Example: If you would have 1024 entries in the entire color map but you only need to store 72 of those entries, this field allows you to start in the middle of the color - map(e.g., position 342).
        afxNat16 nofEntries; // Total number of color map entries included.
        afxInt8 entrySiz; // Establishes the number of bits per entry. Typically 15, 16, 24 or 32-bit values are used.
    } colorMapSpec;
    struct
    {
        afxInt16 originX; // These bytes specify the absolute horizontal coordinate for the lower left corner of the image as it is positioned on a display device having an origin at the lower left of the screen(e.g., the TARGA series).
        afxInt16 originY; // These bytes specify the absolute vertical coordinate for the lower left corner of the image as it is positioned on a display device having an origin at the lower left of the screen(e.g., the TARGA series).
        afxNat16 width; // This field specifies the width of the image in pixels
        afxNat16 height; // This field specifies the height of the image in pixels.
        afxInt8 pixelDepth; // This field indicates the number of bits per pixel. This number includes the Attribute or Alpha channel bits.Common values are 8, 16, 24 and 32 but other pixel depths could be used
        afxInt8 descriptor; // Bits 3-0: These bits specify the number of attribute bits per pixel. Bits 5 & 4: These bits are used to indicate the order in which pixel data is transferred from the file to the screen. Bit 4 is for left - to - right ordering and bit 5 is for topto - bottom ordering as shown below.
    } imageSpec;
};

#pragma pack(pop)

#define _TARGA_RED            0x00001903
#define _TARGA_ALPHA          0x00001906
#define _TARGA_RGB            0x00001907
#define _TARGA_RGBA           0x00001908
#define _TARGA_LUMINANCE      0x00001909
#define _TARGA_SINGLE_CHANNEL _TARGA_RED
#define _TARGA_MAX_DIMENSION  16384

typedef struct _afxTga
{
    afxNat width, height, depth;
    afxByte* data;
    avxFormat fmt; // Format of the TGA image. Can be: _TARGA_RGB, _TARGA_RGBA, _TARGA_LUMINANCE.
    afxRasterUsage usage;
    afxRasterFlags flags;
    afxNat imgCnt;
} _afxTga;

_AVXINL void _AfxTgaSwapColorChannel(afxInt width, afxInt height, afxNat format, afxByte* data)
{
    afxInt i;
    afxByte temp;
    afxInt bytesPerPixel = 3;

    if (!data)
    {
        return;
    }

    if (format == avxFormat_BGRA8 || format == avxFormat_RGBA8)
    {
        bytesPerPixel = 4;
    }
    else
    {
        afxError err = NIL;
        AfxAssert(format == avxFormat_BGR8 || format == avxFormat_RGB8);
    }

    // swap the R and B values to get RGB since the bitmap color format is in BGR
    for (i = 0; i < width * height * bytesPerPixel; i += bytesPerPixel)
    {
        temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }
}

_AVX void _AfxTgaDestroy(afxMmu mmu, _afxTga* tga)
{
    if (tga->data)
    {
        AfxDeallocate(tga->data);
        tga->data = 0;
    }

    tga->width = 0;
    tga->height = 0;
    tga->depth = 0;
    tga->fmt = 0;
}

_AVX afxError _AfxTgaSave(afxMmu mmu, afxStream stream, const _afxTga* tga)
{
    afxError err = AFX_ERR_NONE;
    afxByte bitsPerPixel;
    afxByte* data;

    switch (tga->fmt)
    {
    case avxFormat_R8:
        bitsPerPixel = 8;
        break;
    case avxFormat_RGB8:
    case avxFormat_BGR8:
        bitsPerPixel = 24;
        break;
    case avxFormat_RGBA8:
    case avxFormat_BGRA8:
        bitsPerPixel = 32;
        break;
    default:
        AfxThrowError();
        return FALSE;
    }

    _afxSerializedTargaHdr header = { 0 };
    header.idLength = 0;
    header.colorMapType = 0;
    header.imageType = bitsPerPixel == 8 ? 3 : 2;
    header.colorMapSpec.firstEntryIdx = 0;
    header.colorMapSpec.nofEntries = 0;
    header.colorMapSpec.entrySiz = 0;
    header.imageSpec.originX = 0;
    header.imageSpec.originY = 0;
    header.imageSpec.width = tga->width;
    header.imageSpec.height = tga->height;
    header.imageSpec.pixelDepth = bitsPerPixel;
    header.imageSpec.descriptor = 0;

    AfxWriteStream(stream, sizeof(header), 0, &header);

    afxNat pixelSiz = bitsPerPixel / AFX_BYTE_SIZE;

    if (!(data = AfxAllocate(tga->width * tga->height, pixelSiz, 0, AfxHere()))) AfxThrowError();
    else
    {
        AfxCopy2(data, tga->data, pixelSiz, tga->width * tga->height);

        if (!((tga->fmt == avxFormat_RG8) || (tga->fmt == avxFormat_BGR8) || (tga->fmt == avxFormat_BGRA8)))
            _AfxTgaSwapColorChannel(tga->width, tga->height, tga->fmt, data);

        if (AfxWriteStream(stream, tga->width * tga->height * pixelSiz, 0, data))
            AfxThrowError();

        AfxDeallocate(data);
    }
    return err;
}

_AVX afxError _AfxTgaLoad(afxMmu mmu, afxBool bgrToRgb, afxStream stream, _afxTga* tga)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(stream, afxFcc_IOB);
    AfxAssert(tga);

    _afxSerializedTargaHdr header = { 0 };
    
    if (AfxReadStream(stream, sizeof(header), 0, &header)) AfxThrowError();
    else
    {
        afxBool hasColorMap = FALSE;

        tga->width = header.imageSpec.width;
        tga->height = header.imageSpec.height;
        tga->depth = 1;
        tga->data = 0;
        tga->fmt = 0;
        tga->flags = NIL;
        tga->usage = NIL;

        switch (header.imageType)
        {
        case 1: // indexed RGBA
        {
            hasColorMap = TRUE;
            break;
        }
        case 2: // packed RGB(A)
        case 3:
        {
            break;
        }
        case 9: // indexed RLE
        {
            hasColorMap = TRUE;
            break;
        }
        case 10: // packed RLE
        case 11:
        {
            break;
        }
        default: AfxThrowError(); break;
        }

        if (!err)
        {
            switch (header.imageSpec.pixelDepth)
            {
            case 8:
            case 24:
            case 32:
            {
                if (header.imageSpec.pixelDepth == 32)
                    tga->fmt = bgrToRgb ? avxFormat_RGBA8 : avxFormat_BGRA8;
                else if (header.imageSpec.pixelDepth == 24)
                    tga->fmt = bgrToRgb ? avxFormat_RGB8 : avxFormat_BGR8;
                else
                    tga->fmt = avxFormat_R8;

                break;
            }
            default: AfxThrowError(); break;
            }

            if (!err)
            {
                afxByte* colorMap = NIL;

                if (hasColorMap)
                {
                    afxInt bytesPerPixel = header.colorMapSpec.entrySiz / AFX_BYTE_SIZE;

                    if (!(colorMap = AfxAllocate(header.colorMapSpec.entrySiz, bytesPerPixel, 0, AfxHere()))) AfxThrowError();
                    else
                    {
                        if (AfxReadStream(stream, header.colorMapSpec.nofEntries * bytesPerPixel, 0, colorMap)) AfxThrowError();
                        else
                        {
                            if (header.colorMapSpec.entrySiz == 24 || header.colorMapSpec.entrySiz == 32)
                            {
                                if (bgrToRgb)
                                    _AfxTgaSwapColorChannel(header.colorMapSpec.nofEntries, 1, header.colorMapSpec.entrySiz == 24 ? avxFormat_BGR8 : avxFormat_BGRA8, colorMap);
                            }
                        }

                        if (err)
                        {
                            AfxDeallocate(colorMap);
                            colorMap = NIL;
                        }
                    }
                }

                afxNat pixelSiz = header.imageSpec.pixelDepth / AFX_BYTE_SIZE;

                if (!(tga->data = AfxAllocate((size_t)tga->width * tga->height, pixelSiz, 0, AfxHere()))) AfxThrowError();
                else
                {
                    afxNat i, k;

                    switch (header.imageType)
                    {
                    case 1: // RAW
                    case 2:
                    case 3:
                    {
                        if (AfxReadStream(stream, tga->width * tga->height * pixelSiz, 0, tga->data))
                            AfxThrowError();

                        break;
                    }
                    case 9: // RLE-compressed
                    case 10:
                    case 11:
                    {
                        afxNat pixelsRead = 0;

                        while (pixelsRead < tga->width * tga->height)
                        {
                            afxByte amount;

                            if (AfxReadStream(stream, sizeof(amount), 0, &amount)) AfxThrowError();
                            else
                            {
                                if (amount & 0x80)
                                {
                                    amount &= 0x7F;
                                    amount++;

                                    // read in the rle data
                                    if (AfxReadStream(stream, pixelSiz, 0, &tga->data[pixelsRead * pixelSiz])) AfxThrowError();
                                    else
                                    {
                                        for (i = 1; i < amount; i++)
                                            for (k = 0; k < pixelSiz; k++)
                                                tga->data[(pixelsRead + i) * pixelSiz + k] = tga->data[pixelsRead * pixelSiz + k];
                                    }
                                }
                                else
                                {
                                    amount &= 0x7F;
                                    amount++;

                                    // read in the raw data

                                    if (AfxReadStream(stream, amount * pixelSiz, 0, &tga->data[pixelsRead * pixelSiz]))
                                        AfxThrowError();
                                }
                                pixelsRead += amount;
                            }
                        }
                        break;
                    }
                    default: AfxThrowError(); break;
                    }

                    if (!err)
                    {
                        // swap the color if necessary
                        if (header.imageSpec.pixelDepth == 24 || header.imageSpec.pixelDepth == 32)
                        {
                            if (bgrToRgb)
                                _AfxTgaSwapColorChannel(tga->width, tga->height, tga->fmt, tga->data);
                        }

                        if (hasColorMap)
                        {
                            afxByte* data = tga->data;
                            afxNat bytesPerPixel = header.colorMapSpec.entrySiz / AFX_BYTE_SIZE;

                            // Allocating new memory, as current memory is a look up table index and not a color.

                            if (!(tga->data = AfxAllocate(tga->width * tga->height, bytesPerPixel, 0, AfxHere()))) AfxThrowError();
                            else
                            {
                                if (header.colorMapSpec.entrySiz == 32)
                                    tga->fmt = avxFormat_BGRA8;
                                else if (header.colorMapSpec.entrySiz == 24)
                                    tga->fmt = avxFormat_BGR8;
                                else
                                    tga->fmt = avxFormat_R8;

                                // Copy color values from the color map into the image data.

                                for (i = 0; i < (tga->width * tga->height); i++)
                                    for (k = 0; k < bytesPerPixel; k++)
                                        tga->data[i * bytesPerPixel + k] = colorMap[(header.colorMapSpec.firstEntryIdx + data[i]) * bytesPerPixel + k];

                                // Freeing old data.
                                AfxDeallocate(data);
                                data = 0;
                                //AfxDeallocate(mmu, colorMap);
                                //colorMap = NIL;
                            }
                        }
                    }

                    if (err)
                    {
                        AfxDeallocate(tga->data);
                        tga->data = NIL;
                    }
                }

                if (colorMap)
                {
                    AfxDeallocate(colorMap);
                    colorMap = NIL;
                }
            }
        }
    }
    return err;
}

#if 0
_AVX afxError AfxPrintRaster(afxRaster ras, afxRasterIo const* iop, afxNat lodCnt, afxUri const* uri, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    
    afxFile file;

    if (AfxOpen(uri, afxFileFlag_W, &file)) AfxThrowError();
    else
    {
        afxStream files = AfxGetFileStream(file);

        afxNat maxLodCnt = AfxCountRasterMipmaps(ras);
        AfxAssertRange(maxLodCnt, 0, lodCnt);

        afxRasterIo iop2 = { 0 };

        if (iop)
        {
            afxWhd maxWhd;
            AfxGetRasterExtent(ras, iop->rgn.lodIdx, maxWhd);
            AfxAssertRange(maxLodCnt, iop->rgn.lodIdx, lodCnt);
            AfxAssertRangeWhd(maxWhd, iop->rgn.origin, iop->rgn.whd);
            
            iop2.rgn = iop->rgn;
            iop2.rgn.lodIdx = AfxClamp(iop->rgn.lodIdx, 0, maxLodCnt - 1);

            iop2.rgn.origin[0] = AfxMin(iop->rgn.origin[0], maxWhd[0] - 1);
            iop2.rgn.origin[1] = AfxMin(iop->rgn.origin[1], maxWhd[1] - 1);
            iop2.rgn.origin[2] = AfxMin(iop->rgn.origin[2], maxWhd[2] - 1);

            afxWhd maxRgnWhd;
            AfxWhdSub(maxRgnWhd, maxWhd, iop2.rgn.origin);
            
            iop2.rgn.whd[0] = AfxMin(AfxMax(1, iop->rgn.whd[0]), maxWhd[0]);
            iop2.rgn.whd[1] = AfxMin(AfxMax(1, iop->rgn.whd[1]), maxWhd[1]);
            iop2.rgn.whd[2] = AfxMin(AfxMax(1, iop->rgn.whd[2]), maxWhd[2]);

            AfxWhdClamp(iop2.rgn.whd, iop->rgn.whd, AFX_WHD_IDENTITY, maxRgnWhd);
        }
        else
        {
            AfxGetRasterExtent(ras, 0, iop2.rgn.whd);
        }

        lodCnt = AfxMin(AfxMax(1, lodCnt ? lodCnt : maxLodCnt), maxLodCnt);

        afxTarga tga;
        AfxSetUpTarga(&tga, ras, &iop2, 1);
        AfxBeginTargaOutput(&tga, files);

        iop2.rowStride = tga.rowStride;
        iop2.rowCnt = iop2.rgn.whd[1];
        iop2.offset = AfxAskStreamPosn(files);

        AfxDownloadRaster(ras, 1, &iop2, files, portId);

        tga.data.offset = iop2.offset;
        AfxEndTargaOutput(&tga, files);

        afxDrawContext dctx = AfxGetRasterContext(ras);
        AfxWaitForDrawBridge(dctx, 0, AFX_TIME_INFINITE); // we need to wait for completation before releasing the stream.

        AfxReleaseObjects(1, &file);
    }

    return err;
}
#endif

_AVX afxError _AfxPrintRasterToTarga(afxRaster ras, afxRasterIo const* iop, afxNat lodCnt, afxStream out, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxRasterLayout layout;
    AfxQueryRasterLayout(ras, 0, 0, &layout);
    avxFormat fmt = AfxGetRasterFormat(ras);
    avxFormatDescription pfd;
    AfxDescribePixelFormat(fmt, &pfd);

    // begin file

    afxNat fsegCnt = 1;
    urdSegment fsegs[2] = { 0 };
    urdRoot froot = { 0 };
    urdTrunk_Targa ftrunk = { 0 };

    froot.hdr.fcc = afxFcc_URD;
    ftrunk.trunk.hdr.fcc = afxFcc_TGA;

    // record the TGA 2000 header

    _afxStreamTgaHdr hdr2;

    switch (fmt)
    {
    case avxFormat_R8: // 8-bit
    case avxFormat_S8:
        hdr2.bpp = 8; break;
    case avxFormat_RG8: // 16-bit
    case avxFormat_D16:
    case avxFormat_RGBA4:
    case avxFormat_RGB5A1:
    case avxFormat_R5G6B5:
        hdr2.bpp = 16; break;
    case avxFormat_RGB8: // 24-bit
    case avxFormat_BGR8:
    case avxFormat_RGB8_sRGB:
        hdr2.bpp = 24; break;
    case avxFormat_RGBA8: // 32-bit
    case avxFormat_BGRA8:
    case avxFormat_RGBA8_sRGB:
    case avxFormat_R32f:
    case avxFormat_E5BGR9uf:
    case avxFormat_A2RGB10:
    case avxFormat_D32f:
    case avxFormat_D24S8:
    case avxFormat_X8D24:
        hdr2.bpp = 32; break;
    //default: AfxThrowError(); break;
    }

    hdr2.bpp = pfd.bpp;

    hdr2.idLen = 0;
    hdr2.paletteType = 0;
    hdr2.imgType = hdr2.bpp == 8 ? 3 : 2;
    hdr2.paletteBase = 0;
    hdr2.paletteLen = 0;
    hdr2.paletteBpp = 0;
    hdr2.originOffX = iop->rgn.origin[0];
    hdr2.originOffY = iop->rgn.origin[1];
    hdr2.width = iop->rgn.whd[0];
    hdr2.height = iop->rgn.whd[1];
    hdr2.bpp = hdr2.bpp;
    hdr2.descriptor = 0;

    if (AfxWriteStream(out, sizeof(hdr2), 0, &hdr2))
        AfxThrowError();

    // write data for each segment
        
    fsegs[0].start = AfxAskStreamPosn(out);
        
    afxRasterIo iopTmp = *iop;

    for (afxNat i = 0; i < lodCnt; i++)
    {
        afxRasterLayout lodLayout;
        AfxQueryRasterLayout(ras, i, 0, &lodLayout);

        iopTmp.rgn.lodIdx = i;
        iopTmp.offset = AfxAskStreamPosn(out);
        iopTmp.rowStride = 0;// lodLayout.rowStride;

        if (AfxDownloadRaster(ras, 1, &iopTmp, out, portId))
            AfxThrowError();

        AfxWhdHalf(iopTmp.rgn.origin, iopTmp.rgn.origin);
        AfxWhdHalf(iopTmp.rgn.whd, iopTmp.rgn.whd);
    }

    fsegs[0].encSiz = AfxAskStreamPosn(out) - fsegs[0].start;
    fsegs[0].decSiz = fsegs[0].encSiz;

    //AfxWriteUrdSegment(out, );
    //AfxRecordUrdSegment(out, fsegs[0].start, decSiz, decAlign);

    ftrunk.data.segIdx = 0;
    ftrunk.data.offset = 0;

    // record descriptor for each segment

    ftrunk.trunk.baseSegOff = AfxAskStreamPosn(out);
    ftrunk.trunk.segCnt = fsegCnt;

    for (afxNat i = 0; i < fsegCnt; i++)
    {
        if (AfxWriteStream(out, sizeof(fsegs[0]), 0, &fsegs[i]))
            AfxThrowError();
    }

    // record trunk descriptor

    ftrunk.baseLod = iop->rgn.lodIdx;
    ftrunk.lodCnt = lodCnt;
    ftrunk.fmt = fmt;
    AfxWhdCopy(ftrunk.xyz, iop->rgn.origin);
    AfxWhdCopy(ftrunk.whd, iop->rgn.whd);

    if (AfxTestRasterFlags(ras, afxRasterFlag_CUBEMAP))
        ftrunk.flags |= afxTargaFlag_CUBEMAP;

    if (AfxTestRasterFlags(ras, afxRasterFlag_3D))
        ftrunk.flags |= afxTargaFlag_3D;

    if (AfxWriteStream(out, sizeof(ftrunk), 0, &ftrunk))
        AfxThrowError();

    // record root descriptor

    if (AfxWriteStream(out, sizeof(froot), 0, &froot))
        AfxThrowError();

    return err;
}

_AVX afxError AfxReloadRaster(afxRaster ras, afxNat opCnt, afxRasterIo const ops[], afxNat lodCnt, afxUri const* uri, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxRasterIo iop = { 0 };

    afxWhd whd;
    AfxGetRasterExtent(ras, ops->rgn.lodIdx, whd);

    if (ops)
    {
        iop.rgn = ops->rgn;

        iop.rgn.origin[0] = AfxMin(iop.rgn.origin[0], whd[0] - 1);
        iop.rgn.origin[1] = AfxMin(iop.rgn.origin[1], whd[1] - 1);
        iop.rgn.origin[2] = AfxMin(iop.rgn.origin[2], whd[2] - 1);
        iop.rgn.whd[0] = AfxMin(AfxMax(1, iop.rgn.whd[0]), whd[0]);
        iop.rgn.whd[1] = AfxMin(AfxMax(1, iop.rgn.whd[1]), whd[1]);
        iop.rgn.whd[2] = AfxMin(AfxMax(1, iop.rgn.whd[2]), whd[2]);
    }
    else
    {
        AfxGetRasterExtent(ras, 0, iop.rgn.whd);
    }

    afxStream ios;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &ios);

    if (AfxReloadFile(ios, uri)) AfxThrowError();
    else
    {
        
#if 0
        _afxTgaImg tga = { 0 };

        if (TGA_Load(ios, &tga)) AfxThrowError();
        else
        {
            if (AfxUpdateRaster(ras, &rgn2.dst, tga.data)) AfxThrowError();
            else
            {

            }

            if (tga.data)
                AfxDeallocate(tga.data);
        }
#endif
    }
    AfxReleaseObjects(1, &ios);
    return err;
}

_AVX afxError AfxLoadRasters(afxDrawContext dctx, afxNat cnt, afxRasterInfo const info[], afxUri const uri[], afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(rasters);
    AfxAssert(info);
    AfxAssert(uri);
    AfxAssert(cnt);

    afxNat portId = 0;
    afxStream file = NIL;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);
    
    afxArray tgas;
    afxNat firstArrel;
    AfxAllocateArray(&tgas, cnt, sizeof(_afxTgaImg), (_afxTgaImg[]) { 0 });
    _afxTgaImg* tga = AfxInsertArrayUnits(&tgas, cnt, &firstArrel, NIL);

    if (!tga) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxReloadFile(file, &uri[i])) AfxThrowError();
            else
            {
                if (TGA_Load(file, &tga[i]))
                {
                    AfxThrowError();
                    rasters[i] = NIL;
                }
                else
                {
                    avxFormat fmt;

                    if (tga[i].bpp == 32)
                        fmt = avxFormat_BGRA8;
                    else if (tga[i].bpp == 24)
                        fmt = avxFormat_BGR8;
                    else
                        fmt = avxFormat_R8;

                    afxRasterInfo rasi = { 0 };
                    rasi.fmt = info[i].fmt ? info[i].fmt : fmt;
                    rasi.lodCnt = info[i].lodCnt;
                    rasi.usage = info[i].usage;
                    rasi.flags = info[i].flags;
                    rasi.whd[0] = AfxMax(1, AfxMax(info[i].whd[0], tga[i].whd[0]));
                    rasi.whd[1] = AfxMax(1, AfxMax(info[i].whd[1], tga[i].whd[1]));
                    rasi.whd[2] = AfxMax(1, AfxMax(info[i].whd[2], tga[i].whd[2]));

                    if (AfxAcquireRasters(dctx, 1, &rasi, &rasters[i]))
                    {
                        AfxThrowError();
                        AfxReleaseObjects(i, (void**)rasters);
                    }
                    else
                    {
                        afxRasterIo op = { 0 };
                        op.rgn.whd[0] = tga[i].whd[0];
                        op.rgn.whd[1] = tga[i].whd[1];
                        op.rgn.whd[2] = tga[i].whd[2];
                        op.offset = 0;
                        op.rowStride = tga[i].whd[0];
                        op.rowCnt = tga[i].whd[1];

                        AfxUpdateRaster(rasters[i], 1, &op, tga[i].data, portId);
                    }
                }
            }
        }

        AfxWaitForDrawBridge(dctx, portId, AFX_TIME_INFINITE);

        for (afxNat i = 0; i < tgas.cnt; i++)
        {
            _afxTgaImg* tga = AfxGetArrayUnit(&tgas, i);

            if (tga->data)
                AfxDeallocate(tga->data);
        }
    }

    AfxDeallocateArray(&tgas);

    if (file)
        AfxReleaseObjects(1, &file);

    return err;
}

_AVX afxError AfxAssembleRasters(afxDrawContext dctx, afxRasterInfo const* info, afxUri const* dir, afxNat cnt, afxUri const layers[], afxRaster* ras)
{
    afxError err = AFX_ERR_NONE;
    

    afxStream file = NIL;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    afxNat portId = 0;

    afxUri2048 urib;
    AfxMakeUri2048(&urib, NIL);

    afxArray tgas;
    afxNat firstArrel;
    AfxAllocateArray(&tgas, cnt, sizeof(_afxTgaImg), (_afxTgaImg[]) {0});
    _afxTgaImg* tga = AfxInsertArrayUnits(&tgas, cnt, &firstArrel, NIL);
    
    if (!tga) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(AfxGetUriString(dir)), AfxPushString(AfxGetUriString(&layers[i])));

            if (AfxReloadFile(file, &urib.uri)) AfxThrowError();
            else
            {
                AfxRewindStream(file);

                if (TGA_Load(file, &tga[i])) AfxThrowError();
                else
                {
                    avxFormat fmt;

                    if (tga[i].bpp == 32)
                        fmt = avxFormat_BGRA8;
                    else if (tga[i].bpp == 24)
                        fmt = avxFormat_BGR8;
                    else
                        fmt = avxFormat_R8;

                    if (i == 0)
                    {
                        afxRasterInfo rasi = { 0 };
                        rasi = *info;

                        if (!rasi.fmt)
                            rasi.fmt = fmt;

                        rasi.lodCnt = 1;
                        rasi.whd[0] = tga[i].whd[0];
                        rasi.whd[1] = tga[i].whd[1];
                        rasi.whd[2] = cnt;

                        if (AfxAcquireRasters(dctx, 1, &rasi, ras))
                        {
                            AfxThrowError();
                            break;
                        }
                    }

                    if (!err)
                    {
                        afxRasterIo op = { 0 };
                        op.rgn.origin[2] = i;
                        op.rgn.whd[0] = tga[i].whd[0];
                        op.rgn.whd[1] = tga[i].whd[1];
                        op.rgn.whd[2] = 1;
                        //op.rowStride = tga[i].whd[0] * (tga[i].bpp / AFX_BYTE_SIZE);
                        //op.rowCnt = tga[i].whd[1];

                        if (AfxUpdateRaster(*ras, 1, &op, tga[i].data, portId))
                            AfxThrowError();

                        AfxWaitForDrawBridge(dctx, portId, AFX_TIME_INFINITE);

                        int a = 1;
                    }
                }
            }
        }


        for (afxNat i = 0; i < tgas.cnt; i++)
        {
            _afxTgaImg* tga = AfxGetArrayUnit(&tgas, i);

            if (tga->data)
                AfxDeallocate(tga->data);
        }
    }

    AfxDeallocateArray(&tgas);

    if (file)
        AfxReleaseObjects(1, &file);
    
    return err;
}
