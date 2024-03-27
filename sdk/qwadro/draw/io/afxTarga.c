/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/afxQwadro.h"

_AVX afxString const targaSignature = AFX_STRING(
"\n           :::::::::::     :::     :::::::::   ::::::::      :::               "
"\n               :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:             "
"\n               +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+            "
"\n               +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:           "
"\n               +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+           "
"\n               #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#           "
"\n               ###     ###     ### ###    ###  ########  ###     ###           "
"\n                                                                               "
"\n              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              "
"\n                                                                               "
"\n                               Public Test Build                               "
"\n                   (c) 2017 SIGMA, Engineering In Technology                   "
"\n                                www.sigmaco.org                                "
"\n                                                                               "
);

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

AFX_DEFINE_STRUCT(_afxStreamTargaHdr)
{
    afxNat      baseLod;
    afxNat      lodCnt;
    afxNat      baseLayer;
    afxNat      layerCnt;
    afxWhd      offset;
    afxWhd      whd;

};

AFX_DEFINE_STRUCT(afxTargaLayerHdr)
{
    afxSize     offset;
    afxNat      siz;
};

AFX_DEFINE_STRUCT(afxTargaLodHdr)
{
    afxNat      lodSiz;
    afxNat      layerSiz; // sizeof layer for this LOD
};

AFX_DEFINE_STRUCT(afxTargaFileHdr)
{
    afxFcc      fcc; // tga3
    afxNat      udd[2];
    afxFlags    flags; // 0x01 = Cubemap
    afxNat      codec; // 0x01 = RLE-encoded
    afxNat      fmt;
    afxNat      lodCnt;
    afxNat      layerCnt;
    afxWhd      whd;
    afxNat      rowLen;
    // for each lod
    // sizeof lod is rowLen * whd[1] * whd[2] * layerCnt 
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
                AfxCopy(byteCnt, sizeof(afxByte), buf, &dst[currByte]);
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

afxError TGA_Load(afxStream stream, _afxTgaImg *img)
{
    afxError err = NIL;    
    int i;

    _afxStreamTgaHdr hdr;
    AfxReadStream(stream, sizeof(hdr), 0, &hdr);
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
            if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat32), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
            else
            {
                if (hdr.imgType == 10)
                    DecompressRleChunk(stream, hdr.width, hdr.height, hdr.bpp, img->data);
                else
                    if (AfxReadStream(stream, hdr.width * hdr.height * sizeof(afxNat32), 0, img->data))
                        AfxThrowError();
            }
            break;
        }
        case 24:
        {
            if (!(img->data = AfxAllocate(hdr.width * hdr.height, 3 * sizeof(afxByte), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
            else
            {
                if (hdr.imgType == 10)
                    DecompressRleChunk(stream, hdr.width, hdr.height, hdr.bpp, img->data);
                else
                    if (AfxReadStream(stream, hdr.width * hdr.height * 3, 0, img->data))
                        AfxThrowError();
            }
            break;
        }
        case 16:
        case 15:
        {
            if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat16), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
            else
            {
                if (hdr.imgType == 10)
                    DecompressRleChunk(stream, hdr.width, hdr.height, hdr.bpp, img->data);
                else
                    if (AfxReadStream(stream, sizeof(afxNat16) * hdr.width * hdr.height, 0, img->data))
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
                    if (!(palette = AfxAllocate(hdr.paletteLen, sizeof(afxNat32), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                    else
                    {
                        if (AfxReadStream(stream, hdr.paletteLen * sizeof(afxNat32), 0, palette)) AfxThrowError();
                        else
                        {
                            if (!(buf = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(stream, hdr.width, hdr.height, hdr.bpp, buf);
                                else
                                    if (AfxReadStream(stream, hdr.width * hdr.height, 0, buf))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat32), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
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
                    if (!(palette = AfxAllocate(hdr.paletteLen, 3 * sizeof(afxByte), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                    else
                    {
                        if (AfxReadStream(stream, hdr.paletteLen * 3, 0, palette)) AfxThrowError();
                        else
                        {
                            if (!(buf = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(stream, hdr.width, hdr.height, hdr.bpp, buf);
                                else
                                    if (AfxReadStream(stream, hdr.width * hdr.height, 0, buf))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, 3 * sizeof(afxByte), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
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
                    if (!(palette = AfxAllocate(hdr.paletteLen, sizeof(afxNat16), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                    else
                    {
                        if (AfxReadStream(stream, hdr.paletteLen * sizeof(afxNat16), 0, palette)) AfxThrowError();
                        else
                        {
                            if (!(buf = AfxAllocate(hdr.width * hdr.height, 1, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                            else
                            {
                                if (hdr.imgType == 9)
                                    DecompressRleChunk(stream, hdr.width, hdr.height, hdr.bpp, buf);
                                else
                                    if (AfxReadStream(stream, hdr.width * hdr.height, 0, buf))
                                        AfxThrowError();

                                if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat16), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
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
                if (!(img->data = AfxAllocate(hdr.width * hdr.height, sizeof(afxNat8), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                else
                {
                    if (hdr.imgType == 11)
                        DecompressRleChunk(stream, hdr.width, hdr.height, hdr.bpp, img->data);
                    else
                        if (AfxReadStream(stream, hdr.width * hdr.height * sizeof(afxNat8), 0, img->data))
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

                if (!(buf = AfxAllocate(1, siz, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                else
                {
                    for (i = 0; i < hdr.height; i++)
                        AfxCopy(1, scanline, img->data + i * scanline, buf + (siz - (i + 1) * scanline));

                    AfxCopy(1, siz, buf, img->data);

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

#if 0
_AVX afxError AfxDownloadRasterRegionsToTarga(afxDrawInput din, afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const op = ops[i];


        afxFile file;

        if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { afxFileFlag_W })) AfxThrowError();
        else
        {
            _afxTga im;
            AfxZero(1, sizeof(im), &im);

            //afxWhd extent;
            //AfxGetRasterExtent(tex, 0, extent);
            afxPixelLayout pfd;
            afxPixelFormat fmt = AfxGetRasterFormat(ras);
            AfxDescribePixelFormat(fmt, &pfd);

            if (FALSE == _AfxTgaGen(mem, &im, op.rgn.whd[0], op.rgn.whd[1], op.rgn.whd[2], fmt)) AfxThrowError();
            else
            {
                AfxDumpRasterRegions(ras, 1, &op, im.data);

                if (_AfxTgaSave(mem, AfxGetFileStream(file), &im)) AfxThrowError();
                else
                {

                }
            }

            _AfxTgaDestroy(mem, &im);

            AfxReleaseObjects(1, (void*[]) { file });
        }
    }
    return err;
}

_AVX afxError AfxDownloadRasterToTarga(afxDrawInput din, afxRaster ras, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(uri);
    AfxAssertRange(AfxCountRasterLods(ras), lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(ras), baseImg, imgCnt);

    afxRasterIoOp op = { 0 };
    op.rgn.lodIdx = lodIdx;
    op.rgn.baseLayer = baseImg;
    op.rgn.layerCnt = imgCnt;
    op.rgn.offset[0] = 0;
    op.rgn.offset[1] = 0;
    op.rgn.offset[2] = 0;
    AfxGetRasterExtent(ras, 0, op.rgn.whd);
    op.bufOffset = 0;
    op.bufRowCnt = 0;
    op.bufRowSiz = 0;

    if (AfxDownloadRasterRegionsToTarga(din, ras, 1, &op, uri))
        AfxThrowError();

    return err;
}

_AVX afxError AfxUploadRasterRegionsFromTarga(afxDrawInput din, afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxStream ios = AfxAcquireStream(0, afxIoFlag_RWX, 0, 0);

    for (afxNat i = 0; i < opCnt; i++)
    {
        //afxFile file;

        //if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { afxFileFlag_R })) AfxThrowError();
        if (AfxReloadFile(ios, &uri[i], afxFileFlag_RX)) AfxThrowError();
        else
        {
            _afxTga im;
            AfxZero(1, sizeof(im), &im);

            if (_AfxTgaLoad(mem, TRUE, /*AfxGetFileStream(file)*/ios, &im)) AfxThrowError();
            else
            {
                afxPixelLayout pfd;
                AfxDescribePixelFormat(im.fmt, &pfd);

                afxRasterIoOp op2 = ops[i];
                op2.rgn.layerCnt = 1;
                op2.rgn.whd[0] = AfxMin(op2.rgn.whd[0], im.width);
                op2.rgn.whd[1] = AfxMin(op2.rgn.whd[1], im.height);
                op2.rgn.whd[2] = 1;
                afxNat siz = (im.width * im.height * im.depth * pfd.bpp) / AFX_BYTE_SIZE;

                if (AfxUpdateRasterRegions(ras, 1, &op2, im.data)) AfxThrowError();
                else
                {

                }
            }

            _AfxTgaDestroy(mem, &im);
            //AfxReleaseObjects(1, (void*[]) { file });
        }
    }
    AfxReleaseObjects(1, (void*[]) { ios });
    return err;
}

_AVX afxError AfxUploadRasterFromTarga(afxDrawInput din, afxRaster ras, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(uri);
    AfxAssertRange(AfxCountRasterLods(ras), lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(ras), baseImg, imgCnt);

    afxRasterIoOp op = { 0 };
    op.rgn.lodIdx = lodIdx;
    op.rgn.baseLayer = baseImg;
    op.rgn.layerCnt = imgCnt;
    op.rgn.offset[0] = 0;
    op.rgn.offset[1] = 0;
    op.rgn.offset[2] = 0;
    AfxGetRasterExtent(ras, 0, op.rgn.whd);
    op.bufOffset = 0;
    op.bufRowSiz = 0;
    op.bufRowCnt = 0;

    if (AfxUploadRasterRegionsFromTarga(din, ras, 1, &op, uri))
        AfxThrowError();

    return err;
}

_AVX afxError AfxAcquireRastersFromTarga(afxDrawInput din, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster ras[])
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxStream ios = AfxAcquireStream(0, afxIoFlag_RWX, 0, 0);

    for (afxNat i = 0; i < cnt; i++)
    {
        //afxFile file;

        //if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { afxFileFlag_R })) AfxThrowError();
        if (AfxReloadFile(ios, &uri[i], afxFileFlag_RX)) AfxThrowError();
        else
        {
            _afxTga im;
            AfxZero(1, sizeof(im), &im);

            if (_AfxTgaLoad(mem, TRUE, /*AfxGetFileStream(file)*/ios, &im))
            {
                AfxThrowError();
                ras[i] = NIL;
            }
            else
            {
                afxRasterInfo texi = { 0 };
                texi.fmt = im.fmt;
                texi.lodCnt = 1;
                texi.sampleCnt = 1;
                texi.usage = im.flags;
                texi.layerCnt = im.imgCnt;
                texi.whd[0] = im.width;
                texi.whd[1] = im.height;
                texi.whd[2] = im.depth;

                if (AfxAcquireRasters(dctx, 1, &texi, &ras[i]))
                {
                    AfxThrowError();
                    AfxReleaseObjects(i, (void**)ras);
                }
                else
                {
                    afxPixelLayout pfd;
                    AfxDescribePixelFormat(im.fmt, &pfd);

                    afxRasterIoOp op = { 0 };
                    op.rgn.lodIdx = 0;
                    op.rgn.baseLayer = 0;
                    op.rgn.layerCnt = 1;
                    op.rgn.offset[0] = 0;
                    op.rgn.offset[1] = 0;
                    op.rgn.offset[2] = 0;
                    op.rgn.whd[0] = im.width;
                    op.rgn.whd[1] = im.height;
                    op.rgn.whd[2] = im.depth;
                    op.bufOffset = 0;
                    op.bufRowCnt = 0;
                    op.bufRowSiz = 0;

                    AfxUpdateRasterRegions(ras[i], 1, &op, im.data);
                }
            }

            _AfxTgaDestroy(mem, &im);
            //AfxReleaseObjects(1, (void*[]) { file });
        }
    }
    AfxReleaseObjects(1, (void*[]) { ios });
    return err;
}

_AVX afxError AfxAcquireLayeredRasterFromTarga(afxDrawInput din, afxRasterFlags flags, afxNat layerCnt, afxUri const layerUri[], afxRaster* ras)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxStream ios = AfxAcquireStream(0, afxIoFlag_RWX, 0, 0);

    for (afxNat i = 0; i < layerCnt; i++)
    {
        //afxFile file;

        //if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { afxFileFlag_R })) AfxThrowError();
        if (AfxReloadFile(ios, &layerUri[i], afxFileFlag_RX)) AfxThrowError();
        else
        {
            _afxTga im;
            AfxZero(1, sizeof(im), &im);

            if (_AfxTgaLoad(mem, TRUE, /*AfxGetFileStream(file)*/ios, &im))
            {
                AfxThrowError();
            }
            else
            {
                if (i == 0)
                {
                    afxRasterInfo texi = { 0 };
                    texi.fmt = im.fmt;
                    texi.lodCnt = 1;
                    texi.sampleCnt = 1;
                    texi.usage = flags;
                    texi.layerCnt = layerCnt;
                    texi.whd[0] = im.width;
                    texi.whd[1] = im.height;
                    texi.whd[2] = im.depth;

                    if (AfxAcquireRasters(dctx, 1, &texi, ras))
                    {
                        AfxThrowError();
                        //AfxReleaseObjects(i, (void**)ras);
                    }
                }

                if (!err)
                {
                    afxPixelLayout pfd;
                    AfxDescribePixelFormat(im.fmt, &pfd);

                    afxRasterIoOp op = { 0 };
                    op.rgn.lodIdx = 0;
                    op.rgn.baseLayer = i;
                    op.rgn.layerCnt = 1;
                    op.rgn.offset[0] = 0;
                    op.rgn.offset[1] = 0;
                    op.rgn.offset[2] = 0;
                    op.rgn.whd[0] = im.width;
                    op.rgn.whd[1] = im.height;
                    op.rgn.whd[2] = im.depth;
                    op.bufOffset = 0;
                    op.bufRowSiz = 0;
                    op.bufRowCnt = 0;

                    if (AfxUpdateRasterRegions(*ras, 1, &op, im.data))
                        AfxThrowError();
                }
            }
            _AfxTgaDestroy(mem, &im);
            //AfxReleaseObjects(1, (void*[]) { file });            
        }
    }

    AfxReleaseObjects(1, (void*[]) { ios });
    return err;
}
#endif


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

_AVX _AFXINLINE void _AfxTgaSwapColorChannel(afxInt width, afxInt height, afxNat format, afxByte* data)
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

    if (!(data = AfxAllocate(tga->width * tga->height, pixelSiz, 0, AfxHint()))) AfxThrowError();
    else
    {
        AfxCopy(tga->width * tga->height, pixelSiz, tga->data, data);

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
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
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

                    if (!(colorMap = AfxAllocate(header.colorMapSpec.entrySiz, bytesPerPixel, 0, AfxHint()))) AfxThrowError();
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

                if (!(tga->data = AfxAllocate((size_t)tga->width * tga->height, pixelSiz, 0, AfxHint()))) AfxThrowError();
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

                            if (!(tga->data = AfxAllocate(tga->width * tga->height, bytesPerPixel, 0, AfxHint()))) AfxThrowError();
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

_AVX afxResult _AfxTgaGen(afxMmu mmu, _afxTga* tga, afxInt width, afxInt height, afxInt depth, afxPixelFormat format)
{
    afxInt stride;

    if (!tga || width < 1 || height < 1 || depth < 1)
    {
        return FALSE;
    }

    if (format == afxPixelFormat_R8)
    {
        stride = 1;
    }
    else if (format == afxPixelFormat_BGR8 || format == afxPixelFormat_RGB8)
    {
        stride = 3;
    }
    else if (format == afxPixelFormat_BGRA8 || format == afxPixelFormat_RGBA8)
    {
        stride = 4;
    }
    else
    {
        return FALSE;
    }

    if (!(tga->data = AfxAllocate(width * height * depth, stride * sizeof(afxByte), 0, AfxHint())))
    {
        return FALSE;
    }
    tga->width = width;
    tga->height = height;
    tga->depth = depth;
    tga->fmt = format;

    return TRUE;
}

_AVX afxError AfxPrintRasterRegionsToTarga(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const op = ops[i];

        
        afxStream file;

        if (!(file = AfxOpenFile(&uri[i], afxIoFlag_W))) AfxThrowError();
        else
        {
            _afxTga im;
            AfxZero(1, sizeof(im), &im);

            //afxWhd extent;
            //AfxGetRasterExtent(tex, 0, extent);
            afxPixelLayout pfd;
            afxPixelFormat fmt = AfxGetRasterFormat(ras);
            AfxDescribePixelFormat(fmt, &pfd);
                
            if (FALSE == _AfxTgaGen(mmu, &im, op.rgn.whd[0], op.rgn.whd[1], op.rgn.whd[2], fmt)) AfxThrowError();
            else
            {
                AfxDumpRasterRegions(ras, 1, &op, im.data);

                if (_AfxTgaSave(mmu, file, &im)) AfxThrowError();
                else
                {

                }
            }

            _AfxTgaDestroy(mmu, &im);

            AfxReleaseObjects(1, (void*[]) { file });
        }
    }
    return err;
}

_AVX afxError AfxPrintRasterToTarga(afxRaster ras, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(uri);
    AfxAssertRange(AfxCountRasterLods(ras), lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(ras), baseImg, imgCnt);

    afxRasterIoOp op = { 0 };
    op.rgn.lodIdx = lodIdx;
    op.rgn.baseLayer = baseImg;
    op.rgn.layerCnt = imgCnt;
    op.rgn.offset[0] = 0;
    op.rgn.offset[1] = 0;
    op.rgn.offset[2] = 0;
    AfxGetRasterExtent(ras, 0, op.rgn.whd);
    op.bufOffset = 0;
    op.bufRowCnt = 0;
    op.bufRowSiz = 0;

    if (AfxPrintRasterRegionsToTarga(ras, 1, &op, uri))
        AfxThrowError();

    return err;
}

_AVX afxError AfxFetchRasterRegionsFromTarga(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    
    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxStream ios = AfxAcquireStream(afxIoFlag_R, 0);

    for (afxNat i = 0; i < opCnt; i++)
    {
        //afxFile file;

        //if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { afxFileFlag_R })) AfxThrowError();
        if (AfxReloadFile(ios, &uri[i])) AfxThrowError();
        else
        {
            _afxTga im;
            AfxZero(1, sizeof(im), &im);

            if (_AfxTgaLoad(mmu, TRUE, /*AfxGetFileStream(file)*/ios, &im)) AfxThrowError();
            else
            {
                afxPixelLayout pfd;
                AfxDescribePixelFormat(im.fmt, &pfd);

                afxRasterIoOp op2 = ops[i];
                op2.rgn.layerCnt = 1;
                op2.rgn.whd[0] = AfxMin(op2.rgn.whd[0], im.width);
                op2.rgn.whd[1] = AfxMin(op2.rgn.whd[1], im.height);
                op2.rgn.whd[2] = 1;
                afxNat siz = (im.width * im.height * im.depth * pfd.bpp) / AFX_BYTE_SIZE;

                if (AfxUpdateRasterRegions(ras, 1, &op2, im.data)) AfxThrowError();
                else
                {

                }
            }

            _AfxTgaDestroy(mmu, &im);
            //AfxReleaseObjects(1, (void*[]) { file });
        }
    }
    AfxReleaseObjects(1, (void*[]){ ios });
    return err;
}

_AVX afxError AfxFetchRasterFromTarga(afxRaster ras, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(uri);
    AfxAssertRange(AfxCountRasterLods(ras), lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(ras), baseImg, imgCnt);

    afxRasterIoOp op = { 0 };
    op.rgn.lodIdx = lodIdx;
    op.rgn.baseLayer = baseImg;
    op.rgn.layerCnt = imgCnt;
    op.rgn.offset[0] = 0;
    op.rgn.offset[1] = 0;
    op.rgn.offset[2] = 0;
    AfxGetRasterExtent(ras, 0, op.rgn.whd);
    op.bufOffset = 0;
    op.bufRowCnt = 0;
    op.bufRowSiz = 0;

    if (AfxFetchRasterRegionsFromTarga(ras, 1, &op, uri))
        AfxThrowError();

    return err;
}

_AVX afxError AfxLoadRastersFromTarga(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxStream ios = AfxAcquireStream(afxIoFlag_R, 0);

    for (afxNat i = 0; i < cnt; i++)
    {
        //afxFile file;

        //if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { afxFileFlag_R })) AfxThrowError();
        if (AfxReloadFile(ios, &uri[i])) AfxThrowError();
        else
        {
            _afxTga im;
            AfxZero(1, sizeof(im), &im);

            if (_AfxTgaLoad(mmu, TRUE, /*AfxGetFileStream(file)*/ios, &im))
            {
                AfxThrowError();
                rasters[i] = NIL;
            }
            else
            {
                afxRasterInfo texi = { 0 };
                texi.fmt = im.fmt;
                texi.lodCnt = 1;
                texi.sampleCnt = 1;
                texi.usage = usage | im.usage;
                texi.flags = flags | im.flags;
                texi.layerCnt = im.imgCnt;
                texi.whd[0] = im.width;
                texi.whd[1] = im.height;
                texi.whd[2] = im.depth;

                if (AfxAcquireRasters(dctx, 1, &texi, &rasters[i]))
                {
                    AfxThrowError();
                    AfxReleaseObjects(i, (void**)rasters);
                }
                else
                {
                    afxPixelLayout pfd;
                    AfxDescribePixelFormat(im.fmt, &pfd);

                    afxRasterIoOp op = { 0 };
                    op.rgn.lodIdx = 0;
                    op.rgn.baseLayer = 0;
                    op.rgn.layerCnt = 1;
                    op.rgn.offset[0] = 0;
                    op.rgn.offset[1] = 0;
                    op.rgn.offset[2] = 0;
                    op.rgn.whd[0] = im.width;
                    op.rgn.whd[1] = im.height;
                    op.rgn.whd[2] = im.depth;
                    op.bufOffset = 0;
                    op.bufRowCnt = 0;
                    op.bufRowSiz = 0;

                    AfxUpdateRasterRegions(rasters[i], 1, &op, im.data);
                }
            }

            _AfxTgaDestroy(mmu, &im);
            //AfxReleaseObjects(1, (void*[]) { file });
        }
    }
    AfxReleaseObjects(1, (void*[]){ ios });
    return err;
}

_AVX afxError AfxAssembleRastersFromTarga(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster* ras)
{
    afxError err = AFX_ERR_NONE;

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxStream ios = AfxAcquireStream(afxIoFlag_R, 0);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        //afxFile file;

        //if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { afxFileFlag_R })) AfxThrowError();
        if (AfxReloadFile(ios, &uri[i])) AfxThrowError();
        else
        {
            AfxRewindStream(ios);
            _afxTgaImg tga = { 0 };
            
            if (TGA_Load(ios, &tga)) AfxThrowError();
            else
            {
                afxPixelFormat fmt;

                if (tga.bpp == 32)
                    fmt = afxPixelFormat_BGRA8;
                else if (tga.bpp == 24)
                    fmt = afxPixelFormat_BGR8;
                else
                    fmt = afxPixelFormat_R8;


                if (i == 0)
                {
                    afxRasterInfo texi = { 0 };
                    texi.fmt = fmt;
                    texi.lodCnt = 1;
                    texi.sampleCnt = 1;
                    texi.usage = usage;
                    texi.flags = flags;
                    texi.layerCnt = cnt;
                    texi.whd[0] = tga.whd[0];
                    texi.whd[1] = tga.whd[1];
                    texi.whd[2] = tga.whd[2];

                    if (AfxAcquireRasters(dctx, 1, &texi, ras))
                    {
                        AfxThrowError();
                        //AfxReleaseObjects(i, (void**)ras);
                    }
                }

                if (!err)
                {
                    afxPixelLayout pfd;
                    AfxDescribePixelFormat(fmt, &pfd);

                    afxRasterIoOp op = { 0 };
                    op.rgn.lodIdx = 0;
                    op.rgn.baseLayer = i;
                    op.rgn.layerCnt = 1;
                    op.rgn.offset[0] = 0;
                    op.rgn.offset[1] = 0;
                    op.rgn.offset[2] = 0;
                    op.rgn.whd[0] = tga.whd[0];
                    op.rgn.whd[1] = tga.whd[1];
                    op.rgn.whd[2] = tga.whd[2];
                    op.bufOffset = 0;
                    op.bufRowCnt = 0;
                    op.bufRowSiz = 0;

                    if (AfxUpdateRasterRegions(*ras, 1, &op, tga.data))
                        AfxThrowError();
                }
            //if (tga.data)
                //AfxDeallocate(tga.data);
            }


#if 0

            _afxTga im;
            AfxZero(1, sizeof(im), &im);

            if (_AfxTgaLoad(mmu, TRUE, /*AfxGetFileStream(file)*/ios, &im))
            {
                AfxThrowError();
            }
            else
            {
                if (i == 0)
                {
                    afxRasterInfo texi = { 0 };
                    texi.fmt = im.fmt;
                    texi.lodCnt = 1;
                    texi.sampleCnt = 1;
                    texi.usage = usage;
                    texi.flags = flags;
                    texi.layerCnt = cnt;
                    texi.whd[0] = im.width;
                    texi.whd[1] = im.height;
                    texi.whd[2] = im.depth;

                    if (AfxAcquireRasters(dctx, 1, &texi, ras))
                    {
                        AfxThrowError();
                        //AfxReleaseObjects(i, (void**)ras);
                    }                    
                }
                
                if (!err)
                {
                    afxPixelLayout pfd;
                    AfxDescribePixelFormat(im.fmt, &pfd);

                    afxRasterIoOp op = { 0 };
                    op.rgn.lodIdx = 0;
                    op.rgn.baseLayer = i;
                    op.rgn.layerCnt = 1;
                    op.rgn.offset[0] = 0;
                    op.rgn.offset[1] = 0;
                    op.rgn.offset[2] = 0;
                    op.rgn.whd[0] = im.width;
                    op.rgn.whd[1] = im.height;
                    op.rgn.whd[2] = im.depth;
                    op.bufOffset = 0;
                    op.bufRowCnt = 0;
                    op.bufRowSiz = 0;

                    if (AfxUpdateRasterRegions(*ras, 1, &op, im.data))
                        AfxThrowError();
                }
            }
            _AfxTgaDestroy(mmu, &im);
            //AfxReleaseObjects(1, (void*[]) { file });            
#endif
        }
    }

    AfxReleaseObjects(1, (void*[]){ ios });
    return err;
}
