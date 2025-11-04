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
// This software is part of Advanced Video Graphics Extensions & Experiments.
// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_RASTER_FILE_H
#define AVX_RASTER_FILE_H

#include "qwadro/draw/avxRasterIo.h"

// provisory as Qwadro formats are inherited from Vulkan.

typedef enum avxTgaCodec
{
    avxTgaCodec_NONE,
    avxTgaCodec_RLE8 = 1,
    avxTgaCodec_GDEFLATE,
} avxTgaCodec;

typedef enum avxTgaFlag
{
    // Horizontally inverted. Horizontal origin is top instead of bottom.
    avxTgaFlag_MIRRORED = AFX_BITMASK(0),
    // Vertically inverted. Vertical origin is right instead of left.
    avxTgaFlag_FLIPPED  = AFX_BITMASK(1),
    // Layered image is treated as cubemap.
    avxTgaFlag_CUBEMAP  = AFX_BITMASK(2),
    // Depth is treated as 3D depth instead of layers.
    avxTgaFlag_3D       = AFX_BITMASK(3),
} avxTgaFlags;

#define AFX_TGA_ALIGNMENT (64)
#define AFX_TGA_HDR_SIZ (18)
#define AFX_TGA_EXT_SIZ (AFX_TGA_ALIGNMENT - AFX_TGA_HDR_SIZ) // 64 - 18 = 46
#define AFX_TGA_UDD_SIZ (256 - AFX_TGA_EXT_SIZ - AFX_TGA_HDR_SIZ)

#pragma pack(push, 1)

AFX_DEFINE_UNION(avxRasterFileEx)
{
    struct
    {
        afxUnit8    fcc[4]; // Originally "tga4", a FCC standing for "Targa for Qwadro" file format.
        afxUnit8    idd[18-4]; // originally it was just a padding to force AFX_SIMD_ALIGNMENT right after the header.
    };
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

AFX_STATIC_ASSERT(sizeof(avxRasterFileEx) == 18, "");

AFX_DEFINE_STRUCT(avxRasterFile)
// SIGMA FEDERATION, TECHNICOLOR RASTER FILE HEADER
{
    // The number of bytes contained in SIGMA extension.
    // A value of zero indicates that it is no a SIGMA Targa file.
    // Stride to the next image, if it is a Qwadro TXD.
    afxUnit8        extLen;    
    // The type of color map (if non-zero) included with the image. There are currently 2 defined values for this field: 
    //  0 - indicates that no color-map data is included with this image.
    //  1 - indicates that a color-map is included with this image.
    afxUnit8        palType;
    // The TGA File Format can be used to store Pseudo-Color, True-Color and Direct-Color images of various pixel depths.
    // Truevision has currently defined seven image types: 
    //  0 - No Image Data Included;
    //  1 - Uncompressed, Color-mapped Image;
    //  2 - Uncompressed, True-color Image;
    //  3 - Uncompressed, Black-and-white Image;
    // ... RLE4? AND/XOR? Planar?
    //  9 - RLE8 Color-mapped Image;
    //  10 - RLE8 True-color Image;
    //  11 - RLE8 Black-and-white Image;
    afxUnit8        imgType;
    // Index of the first color map entry. Index refers to the starting entry in loading the color map. 
    // Example: If you would have 1024 entries in the entire color map but you only need to store 72 of those entries, 
    // this field allows you to start in the middle of the color - map(e.g., position 342).
    afxUnit16       palBase;
    // Total number of color map entries included.
    afxUnit16       palSiz;
    // The number of bits per entry. Typically 15, 16, 24 or 32-bit values are used.
    afxUnit8        palBpp;
    // The absolute horizontal coordinate for the lower left corner of the image as it is positioned on a display device having an origin at the lower left of the screen(e.g., the TARGA series).
    afxUnit16       originX;
    // The absolute vertical coordinate for the lower left corner of the image as it is positioned on a display device having an origin at the lower left of the screen(e.g., the TARGA series).
    afxUnit16       originY;
    // The width of the image in pixels
    afxUnit16       width;
    // The height of the image in pixels.
    afxUnit16       height;
    // The number of bits per pixel. This number includes the Attribute or Alpha channel bits. 
    // Common values are 8, 16, 24 and 32 but other pixel depths could be used.
    afxUnit8        bpp;
    // Bits 3-0: These bits specify the number of attribute bits per pixel.
    // Bits 5 & 4: These bits are used to indicate the order in which pixel data is transferred from the file to the screen.
    // Bit 4 is for left-to-right ordering and bit 5 is for top-to-bottom ordering as shown below.
    afxUnit8        descriptor;
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
    avxRasterFileEx idd;
};

#pragma pack(pop)

AFX_STATIC_ASSERT(sizeof(avxRasterFile) == 64, "Header must be 64-byte aligned");

AFX_STATIC_ASSERT(AFX_TEST_ALIGNMENT(sizeof(avxRasterFile), AFX_SIMD_ALIGNMENT), "");
AFX_STATIC_ASSERT(sizeof(avxRasterFile) + AFX_TGA_UDD_SIZ == 256, "");


AVX afxError AvxPrepareRasterFile(avxRasterFile* tga, avxRasterIo* iop, afxUnit lodCnt, avxFormat fmt, avxRasterFlags flags, afxUnit uddSiz);

AVX afxError AvxWriteRasterFile(avxRasterFile* tgai, void* udd, afxUnit uddSiz, afxStream out);

AVX afxError AvxReadRasterFile(avxRasterFile* info, afxStream in);

AVX afxError AvxDecodeRasterFile(avxRasterFile const* info, afxStream in, void* dst);

// TODO: Encode/decode on DPU?

/*
    The AvxLoadRasters() function loads multiple rasters from specified URIs into a drawing system.
*/

AVX afxError            AvxLoadRasters
(
    // The system which will host the new acquired rasters.
    afxDrawSystem       dsys,
    // The number of rasters to be acquired and loaded.
    afxUnit             cnt,
    // An array of structures prescribing the rasters to be acquired.
    avxRasterInfo const info[],
    // An array of URIs locating the source files.
    afxUri const        uri[],
    // An optional array of TGA descriptors to retrieve additional data.
    avxRasterFile       tga[],
    // An array of handles for each acquired raster.
    avxRaster           rasters[]
);

#endif//AVX_RASTER_FILE_H
