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
    afxPixelFormat      fmt;
    afxTargaFlags       flags;
    // chunk data info
    afxUrdReference     data; // where start (above this header) the raster data // for each lod, sizeof lod is rowLen * whd[1] * whd[2] * layerCnt 
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

#pragma pack(pop)

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
    afxNat bkpOff = AfxGetStreamPosn(in);
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
        AfxSeekStreamFromBegin(in, bkpOff);
        _afxStreamTgaHdr hdr2;

        if (AfxReadStream(in, sizeof(hdr2), 0, &hdr2)) AfxThrowError();
        else
        {
            tga->data.offset = AfxGetStreamPosn(in);

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
                tga->fmt = afxPixelFormat_BGRA8;

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
                tga->fmt = afxPixelFormat_BGR8;

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
                tga->fmt = afxPixelFormat_RGBA4;

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
                tga->fmt = hdr2.descriptor & 0x3 ? afxPixelFormat_RGB5A1 : afxPixelFormat_R5G6B5;

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
                    tga->fmt = afxPixelFormat_R8;

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
                        tga->fmt = afxPixelFormat_BGRA8;

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
                        tga->fmt = afxPixelFormat_BGR8;

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
                        tga->fmt = afxPixelFormat_RGBA4;

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
                        tga->fmt = hdr2.descriptor & 0x3 ? afxPixelFormat_RGB5A1 : afxPixelFormat_R5G6B5;

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
                        tga->fmt = afxPixelFormat_R8;

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
    case afxPixelFormat_R8:
    case afxPixelFormat_S8:
        hdr2.bpp = 8; break;
    case afxPixelFormat_RG8:
    case afxPixelFormat_GR8:
    case afxPixelFormat_D16:
        hdr2.bpp = 16; break;
    case afxPixelFormat_RGB8:
    case afxPixelFormat_BGR8:
    case afxPixelFormat_RGB8_SRGB:
    case afxPixelFormat_RGBA4:
    case afxPixelFormat_RGB5A1:
    case afxPixelFormat_R5G6B5:
    case afxPixelFormat_D24:
        hdr2.bpp = 24; break;
    case afxPixelFormat_RGBA8:
    case afxPixelFormat_BGRA8:
    case afxPixelFormat_RGBA8_SRGB:
    case afxPixelFormat_R32F:
    case afxPixelFormat_RGB9E5:
    case afxPixelFormat_RGB10A2:
    case afxPixelFormat_D32:
    case afxPixelFormat_D32F:
    case afxPixelFormat_D24S8:
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
                afxByte *buf = NIL;
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
                            if (!(buf = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, buf);
                                else
                                    if (AfxReadStream(in, hdr.width * hdr.height, 0, buf))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat32), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                                else
                                {
                                    for (i = 0; i < hdr.width * hdr.height; i++)
                                    {
                                        img->data[4 * i + 0] = palette[4 * buf[i] + 0];
                                        img->data[4 * i + 1] = palette[4 * buf[i] + 1];
                                        img->data[4 * i + 2] = palette[4 * buf[i] + 2];
                                        img->data[4 * i + 3] = palette[4 * buf[i] + 3]; // <
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
                            if (!(buf = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, buf);
                                else
                                    if (AfxReadStream(in, hdr.width * hdr.height, 0, buf))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, 3 * sizeof(afxByte), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                                else
                                {
                                    for (i = 0; i < hdr.width * hdr.height; i++)
                                    {
                                        img->data[3 * i + 0] = palette[3 * buf[i] + 0];
                                        img->data[3 * i + 1] = palette[3 * buf[i] + 1];
                                        img->data[3 * i + 2] = palette[3 * buf[i] + 2];
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
                            if (!(buf = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(in, hdr.width, hdr.height, hdr.bpp, buf);
                                else
                                    if (AfxReadStream(in, hdr.width * hdr.height, 0, buf))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat16), AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
                                else
                                {
                                    for (i = 0; i < hdr.width * hdr.height; i++)
                                        ((afxNat16*)img->data)[i] = ((afxNat16*)palette)[buf[i]];

                                    hdr.bpp = 16;
                                }
                            }
                        }
                    }
                    break;
                }
                default: AfxThrowError(); break;
                }
                
                if (buf)
                    AfxDeallocate(buf);

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
    afxPixelFormat fmt; // Format of the TGA image. Can be: _TARGA_RGB, _TARGA_RGBA, _TARGA_LUMINANCE.
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

    if (format == afxPixelFormat_BGRA8 || format == afxPixelFormat_RGBA8)
    {
        bytesPerPixel = 4;
    }
    else
    {
        afxError err = NIL;
        AfxAssert(format == afxPixelFormat_BGR8 || format == afxPixelFormat_RGB8);
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
    case afxPixelFormat_R8:
        bitsPerPixel = 8;
        break;
    case afxPixelFormat_RGB8:
    case afxPixelFormat_BGR8:
        bitsPerPixel = 24;
        break;
    case afxPixelFormat_RGBA8:
    case afxPixelFormat_BGRA8:
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

        if (!((tga->fmt == afxPixelFormat_GR8) || (tga->fmt == afxPixelFormat_BGR8) || (tga->fmt == afxPixelFormat_BGRA8)))
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
                    tga->fmt = bgrToRgb ? afxPixelFormat_RGBA8 : afxPixelFormat_BGRA8;
                else if (header.imageSpec.pixelDepth == 24)
                    tga->fmt = bgrToRgb ? afxPixelFormat_RGB8 : afxPixelFormat_BGR8;
                else
                    tga->fmt = afxPixelFormat_R8;

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
                                    _AfxTgaSwapColorChannel(header.colorMapSpec.nofEntries, 1, header.colorMapSpec.entrySiz == 24 ? afxPixelFormat_BGR8 : afxPixelFormat_BGRA8, colorMap);
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
                                    tga->fmt = afxPixelFormat_BGRA8;
                                else if (header.colorMapSpec.entrySiz == 24)
                                    tga->fmt = afxPixelFormat_BGR8;
                                else
                                    tga->fmt = afxPixelFormat_R8;

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

_AVX afxError AfxPrintRaster(afxRaster ras, afxNat portIdx, afxRasterIo const* op, afxNat lodCnt, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxStream file;

    if (AfxOpenFile(uri, afxIoFlag_W, &file)) AfxThrowError();
    else
    {
        afxRasterIo iop = { 0 };

        if (op)
        {
            afxWhd whd;
            AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
            iop.rgn = op->rgn;

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

        afxTarga tga;
        AfxSetUpTarga(&tga, ras, &iop, 1);
        AfxBeginTargaOutput(&tga, file);

        iop.rowStride = tga.rowStride;
        iop.rowCnt = iop.rgn.whd[1];
        iop.offset = AfxGetStreamPosn(file);

        AfxDownloadRaster(ras, portIdx, 1, &iop, file);

        tga.data.offset = iop.offset;
        AfxEndTargaOutput(&tga, file);

        afxDrawContext dctx = AfxGetRasterContext(ras);
        AfxWaitForDrawBridge(dctx, 0); // we need to wait for completation before releasing the stream.

        AfxReleaseObjects(1, &file);
    }

    return err;
}

_AVX afxError AfxReloadRaster(afxRaster ras, afxRasterIo const* op, afxNat lodCnt, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxRasterIo iop = { 0 };

    afxWhd whd;
    AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);

    if (op)
    {
        iop.rgn = op->rgn;

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
    AfxAcquireStream(0, afxIoFlag_R, 0, &ios);

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

_AVX afxError AfxLoadRasters(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;

    afxNat portIdx = 0;
    afxStream ios;
    AfxAcquireStream(0, afxIoFlag_R, 0, &ios);

    afxArray tgas;
    afxNat firstArrel;
    AfxAllocateArray(&tgas, cnt, sizeof(_afxTgaImg), (_afxTgaImg[]) { 0 });
    _afxTgaImg* tga = AfxInsertArrayUnits(&tgas, cnt, &firstArrel, NIL);

    if (!tga) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxReloadFile(ios, &uri[i])) AfxThrowError();
            else
            {
                if (TGA_Load(ios, &tga[i]))
                {
                    AfxThrowError();
                    rasters[i] = NIL;
                }
                else
                {
                    afxPixelFormat fmt;

                    if (tga[i].bpp == 32)
                        fmt = afxPixelFormat_BGRA8;
                    else if (tga[i].bpp == 24)
                        fmt = afxPixelFormat_BGR8;
                    else
                        fmt = afxPixelFormat_R8;

                    afxRasterInfo rasi = { 0 };
                    rasi.fmt = fmt;
                    rasi.lodCnt = 1;
                    rasi.sampleCnt = 1;
                    rasi.usage = usage;
                    rasi.flags = flags;
                    rasi.whd[0] = tga[i].whd[0];
                    rasi.whd[1] = tga[i].whd[1];
                    rasi.whd[2] = tga[i].whd[2];

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
                        op.rowStride = tga[i].whd[0];
                        op.rowCnt = tga[i].whd[1];

                        AfxUpdateRaster(rasters[i], portIdx, 1, &op, tga[i].data);
                    }
                }
            }
        }

        AfxWaitForDrawBridge(dctx, portIdx);

        for (afxNat i = 0; i < tgas.cnt; i++)
        {
            _afxTgaImg* tga = AfxGetArrayUnit(&tgas, i);

            if (tga->data)
                AfxDeallocate(tga->data);
        }
    }

    AfxDeallocateArray(&tgas);
    AfxReleaseObjects(1, &ios);

    return err;
}

_AVX afxError AfxAssembleRasters(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxUri const* dir, afxNat cnt, afxUri const layers[], afxRaster* ras)
{
    afxError err = AFX_ERR_NONE;
    afxStream ios;
    AfxAcquireStream(0, afxIoFlag_R, 0, &ios);

    afxNat portIdx = 0;

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

            if (AfxReloadFile(ios, &urib.uri)) AfxThrowError();
            else
            {
                AfxRewindStream(ios);

                if (TGA_Load(ios, &tga[i])) AfxThrowError();
                else
                {
                    afxPixelFormat fmt;

                    if (tga[i].bpp == 32)
                        fmt = afxPixelFormat_BGRA8;
                    else if (tga[i].bpp == 24)
                        fmt = afxPixelFormat_BGR8;
                    else
                        fmt = afxPixelFormat_R8;

                    if (i == 0)
                    {
                        afxRasterInfo rasi = { 0 };
                        rasi.fmt = fmt;
                        rasi.lodCnt = 1;
                        rasi.sampleCnt = 1;
                        rasi.usage = usage;
                        rasi.flags = flags;
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
                        op.rowStride = tga[i].whd[0];
                        op.rowCnt = tga[i].whd[1];

                        if (AfxUpdateRaster(*ras, portIdx, 1, &op, tga[i].data))
                            AfxThrowError();

                        int a = 1;
                    }
                }
            }
        }

        AfxWaitForDrawBridge(dctx, portIdx);

        for (afxNat i = 0; i < tgas.cnt; i++)
        {
            _afxTgaImg* tga = AfxGetArrayUnit(&tgas, i);

            if (tga->data)
                AfxDeallocate(tga->data);
        }
    }

    AfxDeallocateArray(&tgas);
    AfxReleaseObjects(1, &ios);
    
    return err;
}