/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // TECHNICOLOR TARGA, ADVANCED RASTER GRAPHICS ARCHIVE                      //
//////////////////////////////////////////////////////////////////////////////

// This code is part of Qwadro Draw System <https://sigmaco.org/qwadro>
// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_RASTER_FILE_H
#define AVX_RASTER_FILE_H

#include "qwadro/inc/draw/avxRaster.h"

// provisory as Qwadro formats are inherited from Vulkan.

typedef enum avxTgaCodec
{
    avxTgaCodec_NONE,
    avxTgaCodec_RLE8 = 1,
    avxTgaCodec_GDEFLATE,
} avxTgaCodec;

typedef enum avxTgaFlag
{
    avxTgaFlag_X_FLIPPED    = AFX_BITMASK(0), // horizontal origin is top to bottom.
    avxTgaFlag_Y_FLIPPED    = AFX_BITMASK(1), // vertical origin is right to left.
    avxTgaFlag_CUBEMAP      = AFX_BITMASK(2), // layered image is treated as cubemap.
    avxTgaFlag_3D           = AFX_BITMASK(3), // depth is treated as 3D depth instead of layers.
} avxTgaFlags;

#define AFX_TGA_ALIGNMENT (64)
#define AFX_TGA_HDR_SIZ (18)
#define AFX_TGA_EXT_SIZ (AFX_TGA_ALIGNMENT - AFX_TGA_HDR_SIZ) // 64 - 18 = 46
#define AFX_TGA_UDD_SIZ (256 - AFX_TGA_EXT_SIZ - AFX_TGA_HDR_SIZ)

#pragma pack(push, 1)

AFX_DEFINE_UNION(avxRasterFileUdd)
{
    afxUnit8        fcc[4]; // Originally "tga4", a FCC standing for "Targa for Qwadro" file format.
    afxUnit8        idd[18]; // originally it was just a padding to force AFX_SIMD_ALIGNMENT right after the header.
    struct
    
    {
        afxUnit8    fcc[4]; // Originally "tga4", a FCC standing for "Targa for Qwadro" file format.
        afxUnit16   hotspotX;
        afxUnit16   hotspotY;
    } icon;
    struct
    // Based on PC Screen Font (PSF) 2.
    // Variable glyph width and height.
    // Arbitrary number of glyphs.
    // Full Unicode mapping.
    // For a 16x16 glyph: 16 rows x 2 bytes per row = 32 bytes per glyph. Each row is packed left to right, MSB first.
    {
        afxUnit8    fcc[4]; // Originally "tga4", a FCC standing for "Targa for Qwadro" file format.
        afxUnit32   flags; // Flags (0 = no Unicode table)
        afxUnit16   glyphCnt; // Number of glyphs
        afxUnit8    glyphStride; // bytes per glyph.
        afxUnit8    glyphHeight; // Glyph height in pixels
        afxUnit8    glyphWidth; // Glyph width in pixels
    } font;
};

AFX_STATIC_ASSERT(sizeof(avxRasterFileUdd) == 18, "");

AFX_DEFINE_STRUCT(avxRasterFile)
// SIGMA FEDERATION, TECHNICOLOR RASTER FILE HEADER
{
    // TRUEVISION INC., TGA FILE HEADER
    afxUnit8        extLen; // The number of bytes contained in SIGMA extension. A value of zero indicates that it is no a SIGMA Targa file.
    afxUnit8        palType; // The type of color map (if non-zero) included with the image. There are currently 2 defined values for this field: 0 - indicates that no color - map data is included with this image. 1 - indicates that a color - map is included with this image.
    afxUnit8        imgType; // The TGA File Format can be used to store Pseudo-Color, True-Color and Direct-Color images of various pixel depths.
    // Truevision has currently defined seven image types: 0 - No Image Data Included; 1 - Uncompressed, Color-mapped Image; 2 - Uncompressed, True-color Image; 3 - Uncompressed, Black-and-white Image; 9 - Run-length encoded, Color-mapped Image; 10 - Run-length encoded, True-color Image; 11 - Run-length encoded, Black-and-white Image;
    afxUnit16       palBase; // Index of the first color map entry. Index refers to the starting entry in loading the color map. Example: If you would have 1024 entries in the entire color map but you only need to store 72 of those entries, this field allows you to start in the middle of the color - map(e.g., position 342).
    afxUnit16       palSiz; // Total number of color map entries included.
    afxUnit8        palBpp; // The number of bits per entry. Typically 15, 16, 24 or 32-bit values are used.
    afxUnit16       originX; // The absolute horizontal coordinate for the lower left corner of the image as it is positioned on a display device having an origin at the lower left of the screen(e.g., the TARGA series).
    afxUnit16       originY; // The absolute vertical coordinate for the lower left corner of the image as it is positioned on a display device having an origin at the lower left of the screen(e.g., the TARGA series).
    afxUnit16       width; // The width of the image in pixels
    afxUnit16       height; // The height of the image in pixels.
    afxUnit8        bpp; // The number of bits per pixel. This number includes the Attribute or Alpha channel bits. Common values are 8, 16, 24 and 32 but other pixel depths could be used
    afxUnit8        descriptor; // Bits 3-0: These bits specify the number of attribute bits per pixel. Bits 5 & 4: These bits are used to indicate the order in which pixel data is transferred from the file to the screen. Bit 4 is for left - to - right ordering and bit 5 is for topto - bottom ordering as shown below.
    // SIGMA FEDERATION, RASTER FILE AMENDMENT
    afxUnit16       flags;
    afxUnit16       codec; // codec used to compress
    afxUnit32       encSiz; // compressed size
    afxUnit32       dataOff; // where start (above this header) the raster data // for each lod, sizeof lod is rowLen * whd[1] * whd[2] * layerCnt 
    afxUnit32       decSiz; // uncompressed size
    afxUnit16       rowStride; // bytes per row/scanline (W usually aligned/padded to some memory boundary)
    afxUnit16       rowsPerImg;
    afxUnit8        baseLod;
    afxUnit8        lodCnt;
    afxUnit16       originZ;
    afxUnit16       depth;
    afxUnit16       fmt;
    // data info
    avxRasterFileUdd idd;
};

#pragma pack(pop)

AFX_STATIC_ASSERT(sizeof(avxRasterFile) == 64, "");
// Data start must be aligned to AVX_RASTER_ALIGNMENT.
AFX_STATIC_ASSERT((sizeof(avxRasterFile)) % AVX_RASTER_ALIGNMENT, "");

AFX_STATIC_ASSERT(AFX_TEST_ALIGNMENT(sizeof(avxRasterFile), AFX_SIMD_ALIGNMENT), "");
AFX_STATIC_ASSERT(sizeof(avxRasterFile) + AFX_TGA_UDD_SIZ == 256, "");


AVX afxError AfxPrepareRasterFile(avxRasterFile* tga, avxRasterIo* iop, afxUnit lodCnt, avxFormat fmt, avxRasterFlags flags, afxUnit uddSiz);

AVX afxError AfxWriteRasterFile(avxRasterFile* tgai, void* udd, afxUnit uddSiz, afxStream out);

AVX afxError AfxReadRasterFile(avxRasterFile* info, afxStream in);

AVX afxError AfxDecodeRasterFile(avxRasterFile const* info, afxStream in, void* dst);

// TODO: Encode/decode on DPU?

#endif//AVX_RASTER_FILE_H
