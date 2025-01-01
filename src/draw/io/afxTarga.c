/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#include "../impl/avxImplementation.h"
#include "qwadro/inc/draw/io/afxTarga.h"

static_assert(sizeof(avxFormat) == sizeof(afxUnit32), "");
static_assert(sizeof(afxTargaFlags) == sizeof(afxUnit32), "");
static_assert(sizeof(afxTargaCodec) == sizeof(afxUnit32), "");

#if 0
void LoadRLERow(afxTargaFile* tga, afxStream kIst, afxByte* dst, afxUnit uiDestSize)
{
    // DO NOT clear m_uiRLECount or m_bRLEncodedRun here - an RLE run can cross over multiple scanlines, so a run may cross more than one call to this function

    afxUnit uiPixelCol = 0;
    while (uiPixelCol < tga->width)
    {
        if (m_uiRLECount == 0)
        {
            // Have to restart run.
            afxByte ucTag = 0;
            NiStreamLoadBinary(kIst, (char *)&ucTag, 1);

            m_bRLEncodedRun = (ucTag & 0x80) ? TRUE : FALSE;

            if (ucTag & 0x80)
            {
                m_bRLEncodedRun = TRUE;
                m_uiRLECount = ucTag - 127; // Single pixel replicated

                // get pixel to be replicated
                NiStreamLoadBinary(kIst, (char *)m_aucRLEBits, m_uiBytesPerPixel);
            }
            else
            {
                m_bRLEncodedRun = FALSE;
                m_uiRLECount = ucTag + 1; // Stream of unencoded pixels
            }
        }

        // Have already read count and, if the run is encoded, the pixel that defines the run as well.

        // now copy the run into as many pixels as possible - either the rest of the row, or the entire run, whichever is shorter...
        afxUnit uiRunToCopy = tga->width - uiPixelCol;
        
        if (m_uiRLECount < uiRunToCopy)
            uiRunToCopy = m_uiRLECount;

        AFX_ASSERT(uiRunToCopy > 0);
        AFX_ASSERT(uiRunToCopy <= m_uiRLECount);
        AFX_ASSERT(uiRunToCopy <= (tga->width - uiPixelCol));

        if (m_bRLEncodedRun) // Encoded pixel - copy
        {
            for (unsigned int j = 0; j < uiRunToCopy; j++)
            {
                AfxCopy(dst, uiDestSize, m_aucRLEBits, m_uiBytesPerPixel);
                dst += m_uiBytesPerPixel;
            }
        }
        else  // Unencoded pixel - read next set of pixels
        {
            NiStreamLoadBinary(kIst, (char *)dst, m_uiBytesPerPixel * uiRunToCopy);
            dst += m_uiBytesPerPixel * uiRunToCopy;
        }

        // Decrease remaining run length and remaining collumn pixels
        m_uiRLECount -= uiRunToCopy;
        uiPixelCol += uiRunToCopy;
    }

    AFX_ASSERT(uiPixelCol == tga->width);
}

void Unpack4BitSourceRowPal(afxUnit w, afxUnit palBase, afxByte* src, afxByte* dst)
{
    afxError err = NIL;
    // If width is odd, then assert, since that format is not supported.
    AFX_ASSERT(!(w & 0x1));

    // Assume destination is 4-bit PAL
    // We divide the width in half to reflect the 2 pixels per byte.
    for (afxUnit i = 0; i < (afxUnit)(w >> 1); i++)
    {
        afxByte ucEntryValue = (afxByte)(src[0] - palBase);
        src++;
        *dst++ = ucEntryValue;
    }
}

void Unpack8BitSourceRowPal(afxUnit w, afxUnit palBase, afxByte* src, afxByte* dst)
{
    // Assume destination is 8-bit PAL
    for (afxUnit i = 0; i < w; i++)
    {
        afxByte ucEntryValue = (afxByte)(src[0] - palBase);
        src++;
        *dst++ = ucEntryValue;
    }
}

void Unpack8BitSourceRowGray(afxUnit w, afxByte* src, afxByte* dst)
{
    // Assume destination is 24-bit RGB
    for (afxUnit i = 0; i < w; i++)
    {
        // decode the 8-bit grayscale data into the destination R, G, and B values
        dst[0] = dst[1] = dst[2] = *src;
        dst += 3;
        src++;
    }
}

void Unpack16BitSourceRowRGB(afxUnit w, afxByte* src, afxByte* dst)
{
    // Assume destination is 24-bit RGB

    for (afxUnit i = 0; i < w; i++)
    {
        // decode the 16-bit data into the destination R, G and B values
        // We cannot just treat each pixel as a short, as this would not work across platforms
        *(dst++) = (src[1] & 0x7C) << 1;
        *(dst++) = ((src[1] & 0x03) << 6) + ((src[0] & 0xE0) >> 2);
        *(dst++) = (src[0] & 0x1F) << 3;
        src += 2;
    }
}

void Unpack16BitSourceRowIndexed(afxUnit w, afxByte* src, afxByte* dst)
{
    // Assume destination is 24-bit RGB

    for (afxUnit i = 0; i < w; i++)
    {
        // decode the 16-bit data into the destination
        const NiPalette::PaletteEntry& kEntry = m_pkColorMap[(src[1] << 8) + src[0] - m_usMinPalIndex];

        *(dst++) = kEntry.m_ucRed;
        *(dst++) = kEntry.m_ucGreen;
        *(dst++) = kEntry.m_ucBlue;
        src += 2;
    }
}

void Unpack16BitSourceRowIndexedAlpha(afxUnit w, afxByte* src, afxByte* dst)
{
    // Assume destination is 24-bit RGBA

    for (afxUnit i = 0; i < w; i++)
    {
        // decode the 16-bit data into the destination
        const NiPalette::PaletteEntry& kEntry = m_pkColorMap[(src[1] << 8) + src[0] - m_usMinPalIndex];
        *(dst++) = kEntry.m_ucRed;
        *(dst++) = kEntry.m_ucGreen;
        *(dst++) = kEntry.m_ucBlue;
        *(dst++) = kEntry.m_ucAlpha;
        src += 2;
    }
}

void Unpack24BitSourceRow(afxUnit w, afxByte* src, afxByte* dst)
{
    // Assume destination is 24-bit RGB

    for (afxUnit i = 0; i < w; i++)
    {
        *(dst++) = *(src + 2);
        *(dst++) = *(src + 1);
        *(dst++) = *(src);
        src += 3;
    }
}

void Unpack32BitSourceRow(afxUnit w, afxByte* src, afxByte* dst)
{
    // Assume destination is 32-bit RGBA

    for (afxUnit i = 0; i < w; i++)
    {
        *(dst++) = *(src + 2);
        *(dst++) = *(src + 1);
        *(dst++) = *(src);
        *(dst++) = *(src + 3);
        src += 4;
    }
}
#endif

void DecompressRleChunk(afxStream stream, afxUnit width, afxUnit height, afxUnit bpp, afxByte *dst)
{
    afxByte byteCnt = bpp >> 3;
    afxUnit siz = width * height * byteCnt;
    afxUnit currByte = 0;

    while (currByte < siz)
    {
        afxByte hdr;
        AfxReadStream(stream, sizeof(afxByte), 0, &hdr);

        afxUnit runLen = (hdr & 0x7F) + 1;

        if (hdr & 0x80) // high bit is set when it's a run-length packet.
        {
            afxByte buf[4];
            AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, buf);

            for (afxUnit i = 0; i < runLen; i++)
            {
                AfxCopy2(&dst[currByte], buf, sizeof(afxByte), byteCnt);
                currByte += byteCnt;
            }
        }
        else
        {
            // Raw packet

            for (afxUnit i = 0; i < runLen; i++)
            {
                AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, &dst[currByte]);
                currByte += byteCnt;
            }
        }
    }
}

void DecodeRle8(afxUnit w, afxUnit h, afxByte const* in, afxByte* out)
{
    afxUnit wxh = w * h;
    afxUnit i = 0; // Index in the output buffer

    while (i < wxh)
    {
        afxByte hdr = *in++;
        afxUnit runLen = (hdr & 0x7F) + 1; // Number of pixels to decode

        if (hdr & 0x80) // If the high bit is set, it's a run-length packet. Else case, it is raw packet.
        {
            afxByte data = *in++;
            // Fill run-length with the same color
            for (afxUnit j = 0; j < runLen && i < wxh; ++j)
                out[i++] = data;
        }
        else for (afxUnit j = 0; j < runLen && i < wxh; ++j)
            out[i++] = *in++;
    }
}

_AVX afxError AfxWriteTarga(afxStream out, afxRasterIo* iop, afxUnit lodCnt, avxFormat fmt, afxRasterFlags flags, afxUnit uddSiz, void const* udd)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(!uddSiz || udd);
    AFX_ASSERT(iop);
    AFX_ASSERT(fmt);

    avxFormatDescription pfd;
    AfxDescribePixelFormat(fmt, &pfd);

    // begin file
    // record the TGA 2000 header

    afxUnit uddSiz2 = AFX_ALIGNED_SIZE(uddSiz, AFX_SIMD_ALIGNMENT);
    afxUnit w = AfxMax(pfd.bcWh[0], AFX_ALIGNED_SIZE(iop->rgn.extent.w, pfd.bcWh[0]));
    afxUnit h = AfxMax(pfd.bcWh[1], AFX_ALIGNED_SIZE(iop->rgn.extent.h, pfd.bcWh[1]));
    iop->rgn.extent.w = w;
    iop->rgn.extent.h = h;
    iop->rgn.extent.d = AfxMax(1, iop->rgn.extent.d);
    iop->rgn.origin.x = AFX_ALIGNED_SIZE(iop->rgn.origin.x, pfd.bcWh[0]);
    iop->rgn.origin.y = AFX_ALIGNED_SIZE(iop->rgn.origin.y, pfd.bcWh[1]);
    iop->rgn.origin.z = iop->rgn.origin.z;

    // ignored for block compressed types.
    iop->rowStride = w < iop->rowStride ? iop->rowStride : AFX_ALIGNED_SIZE(w * pfd.stride, AFX_SIMD_ALIGNMENT);
    iop->rowsPerImg = iop->rowsPerImg < h ? h : iop->rowsPerImg;
    
    afxTargaFile tgai = { 0 };
    tgai.extFcc[0] = 't';
    tgai.extFcc[1] = 'g';
    tgai.extFcc[2] = 'a';
    tgai.extFcc[3] = '4';
    tgai.extLen = (AFX_TGA_HDR_SIZ + AFX_TGA_EXT_SIZ) + uddSiz2;
    tgai.palType = 0;
    tgai.imgType = 2;
    tgai.palBase = 0;
    tgai.palSiz = 0;
    tgai.palBpp = 0;
    tgai.originX = iop->rgn.origin.x;
    tgai.originY = iop->rgn.origin.y;
    tgai.originZ = iop->rgn.origin.z;
    tgai.width = w;
    tgai.height = h;
    tgai.depth = iop->rgn.extent.d;
    tgai.lodCnt = AfxMax(1, lodCnt);
    tgai.baseLod = iop->rgn.lodIdx;
    tgai.fmt = fmt ? fmt : avxFormat_BGRA8un;
    tgai.bpp = 32 >= pfd.bpp ? pfd.bpp : pfd.bpc[0];// pfd.bpp / pfd.bcWh[0];
    tgai.descriptor = 0;

    if (flags & afxRasterFlag_CUBEMAP)
        tgai.flags |= afxTargaFlag_CUBEMAP;

    if (flags & afxRasterFlag_3D)
        tgai.flags |= afxTargaFlag_3D;

    // ignored for block compressed types.
    tgai.rowStride = iop->rowStride;
    tgai.rowsPerImg = iop->rowsPerImg;
    //tgai.rowStride = tgai.rowStride > w ? tgai.rowStride : AFX_ALIGNED_SIZE(w, AFX_SIMD_ALIGNMENT);
    //tgai.rowsPerImg = tgai.rowsPerImg > h ? tgai.rowsPerImg : h;

    tgai.dataOff = iop->offset + AFX_TGA_HDR_SIZ + AFX_TGA_EXT_SIZ + uddSiz2;

    if (uddSiz)
        AfxCopy(tgai.udd, udd, uddSiz2);

    if (AfxWriteStream(out, AFX_TGA_HDR_SIZ + AFX_TGA_EXT_SIZ + uddSiz2, 0, &tgai))
        AfxThrowError();

    AFX_ASSERT(AfxAskStreamPosn(out) == 64 + uddSiz2);

    return err;
}

_AVX afxError AfxReadTarga(afxStream in, afxTargaFile* info)
{
    afxError err = NIL;
    afxUnit i;
    
    afxBool legacy = TRUE;

    afxTargaFile tgai;
    if (AfxReadStream(in, AFX_TGA_HDR_SIZ, 0, &tgai))
    {
        AfxThrowError();
        return err;
    }

    if (tgai.extLen >= AFX_TGA_EXT_SIZ)
    {
        if (AfxReadStream(in, AFX_TGA_EXT_SIZ, 0, &tgai.extFcc[0]))
        {
            AfxThrowError();
            return err;
        }

        if ((tgai.extFcc[0] != 't') || (tgai.extFcc[1] != 'g') || (tgai.extFcc[2] != 'a') || (tgai.extFcc[3] != '4'))
        {
            if (AfxAdvanceStream(in, tgai.extLen))
                AfxThrowError();
        }
        else
        {
            *info = tgai;

            info->fmt = tgai.fmt;
            info->flags = tgai.flags;
            info->originX = tgai.originX;
            info->originY = tgai.originY;
            info->originZ = tgai.originZ;
            info->width = tgai.width;
            info->height = tgai.height;
            info->depth = tgai.depth;
            info->baseLod = tgai.baseLod;
            info->lodCnt = tgai.lodCnt;

            info->dataOff = tgai.dataOff;
            info->codec = tgai.codec;
            info->encodedSiz = tgai.encodedSiz;
            info->decodedSiz = tgai.decodedSiz;
            info->rowStride = tgai.rowStride;
            info->rowsPerImg = tgai.rowsPerImg;

            legacy = FALSE;
        }
    }
    
    if (legacy)
    {
        *info = tgai;

        info->originX = tgai.originX;
        info->originY = tgai.originY;
        info->originZ = 0;
        info->width = tgai.width;
        info->height = tgai.height;
        info->depth = 1;
        info->baseLod = 0;
        info->lodCnt = 1;
        info->fmt = NIL;
        info->flags = NIL;

        info->rowStride = tgai.width;
        info->rowsPerImg = tgai.height;
        info->dataOff = AfxAskStreamPosn(in);
        info->decodedSiz = 0;
        info->encodedSiz = 0;
        info->codec = (tgai.imgType >= 9 && 11 <= tgai.imgType) ? afxTargaCodec_RLE8 : afxTargaCodec_NONE;

        afxUnit8 alphaBits = tgai.descriptor & 0x0F;
        
        switch (tgai.imgType)
        {
        case 2: // DATA TYPE 2 - TRUE-COLOR IMAGES
        case 10: // DATA TYPE 10 - RUN-LENGTH ENCODED (RLE), TRUE-COLOR IMAGES
        {
            // packed raster data

            afxUnit bppAligned = (AFX_ALIGNED_SIZE(tgai.bpp, AFX_BYTE_SIZE));
            afxUnit pixStride = (AFX_ALIGNED_SIZE(bppAligned, AFX_BYTE_SIZE) / AFX_BYTE_SIZE);
            afxUnit rowStride = info->width * pixStride;

            info->rowStride = rowStride;
            info->rowsPerImg = info->height;
            info->decodedSiz = info->rowsPerImg * AFX_ALIGNED_SIZE(info->rowStride, AFX_SIMD_ALIGNMENT);
            info->encodedSiz = info->rowsPerImg * info->rowStride;
            
            switch (tgai.bpp)
            {
            case 32: info->fmt = avxFormat_BGRA8; break;
            case 24: info->fmt = avxFormat_BGR8; break;
            case 16: info->fmt = avxFormat_BGRA4; break;
            case 15: info->fmt = avxFormat_BGR5A1; break;
            case 12: info->fmt = avxFormat_A2BGR10; break;
            default: AfxThrowError(); break;
            }

            break;
        }
        case 3: // DATA TYPE 3 - BLACK AND WHITE (UNMAPPED) IMAGES
        case 11: // DATA TYPE 11 - RUN-LENGTH ENCODED (RLE), BLACK AND WHITE IMAGES
        {
            // packed raster data

            afxUnit bppAligned = (AFX_ALIGNED_SIZE(tgai.bpp, AFX_BYTE_SIZE));
            afxUnit pixStride = (AFX_ALIGNED_SIZE(bppAligned, AFX_BYTE_SIZE) / AFX_BYTE_SIZE);
            afxUnit rowStride = info->width * pixStride;

            info->rowStride = rowStride;
            info->rowsPerImg = info->height;
            info->decodedSiz = info->rowsPerImg * AFX_ALIGNED_SIZE(info->rowStride, AFX_SIMD_ALIGNMENT);
            info->encodedSiz = info->rowsPerImg * info->rowStride;
            
            switch (tgai.bpp)
            {
            case  8: info->fmt = avxFormat_R8; break;
            case 16: info->fmt = avxFormat_R16; break;
            case 24: info->fmt = avxFormat_D24; break;
            case 32: info->fmt = avxFormat_R32f; break;
            case 64: info->fmt = avxFormat_R64f; break;
            default: AfxThrowError(); break;
            }

            break;
        }
        case 1: // DATA TYPE 1 - COLOR-MAPPED IMAGES
        case 9: // DATA TYPE 9 - RUN-LENGTH ENCODED (RLE), COLOR-MAPPED IMAGES
        {
            // palletized/indexed raster data

            afxUnit bppAligned = (AFX_ALIGNED_SIZE(tgai.palBpp, AFX_BYTE_SIZE));
            afxUnit pixStride = (AFX_ALIGNED_SIZE(bppAligned, AFX_BYTE_SIZE) / AFX_BYTE_SIZE);
            //info->rowStride = info->width * pixStride;
            //info->imgStride = info->height * info->rowStride;
            //info->decSiz = info->height * info->rowStride;
            //info->encSiz = info->height * info->rowStride;

            switch (tgai.palBpp)
            {
            case 32: info->fmt = avxFormat_BGRA8; break;
            case 24: info->fmt = avxFormat_BGR8; break;
            case 16: info->fmt = avxFormat_BGRA4; break;
            case 15: info->fmt = avxFormat_BGR5A1; break;
            case 12: info->fmt = avxFormat_A2BGR10; break;
            default: AfxThrowError(); break;
            }

            break;
        }
        case 0: // DATA TYPE 0 - PLACEHOLDER, NO DATA RESIDENT.
        {
            break;
        }
        default: AfxThrowError();
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
                afxUnit scanline = hdr.width * (hdr.bpp >> 3);
                afxUnit siz = scanline * hdr.height;
                afxByte *buf;

                if (!(buf = AfxAllocate(1, siz, AFX_SIMD_ALIGNMENT, AfxHere()))) AfxThrowError();
                else
                {
                    for (i = 0; i < hdr.height; i++)
                        AfxCopy2(1, scanline, img->data + i * scanline, buf + (siz - (i + 1) * scanline));

                    AfxCopy2(1, siz, buf, img->data);

                    AfxDeallocate(buf);
                }
            }
#endif
        }
    }
    return err;
}

_AVX afxError AfxDecodeTarga(afxStream in, afxTargaFile const* meta, void* dst)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(meta);
    AFX_ASSERT(meta->rowStride);
    AFX_ASSERT(meta->rowsPerImg);

    AfxSeekStream(in, meta->dataOff, afxSeekOrigin_BEGIN);

    AFX_ASSERT(dst);
    afxByte *data = dst;

    switch (meta->imgType)
    {
    case 2: // DATA TYPE 2 - TRUE-COLOR IMAGES
    {
        // packed raster data
        if (AfxReadStream(in, meta->rowsPerImg * meta->rowStride, 0, data))
            AfxThrowError();

        break;
    }
    case 10: // DATA TYPE 10 - RUN-LENGTH ENCODED (RLE), TRUE-COLOR IMAGES
    {
        // packed raster data
        DecompressRleChunk(in, meta->width, meta->height, meta->bpp, data);
        break;
    }
    case 3: // DATA TYPE 3 - BLACK AND WHITE (UNMAPPED) IMAGES
    {
        // packed raster data
        if (AfxReadStream(in, meta->rowsPerImg * meta->rowStride, 0, data))
            AfxThrowError();

        break;
    }
    case 11: // DATA TYPE 11 - RUN-LENGTH ENCODED (RLE), BLACK AND WHITE IMAGES
    {
        // packed raster data
        DecompressRleChunk(in, meta->width, meta->height, meta->bpp, data);
        break;
    }
    case 1: // DATA TYPE 1 - COLOR-MAPPED IMAGES
    case 9: // DATA TYPE 9 - RUN-LENGTH ENCODED (RLE), COLOR-MAPPED IMAGES
    {
        // palletized/indexed raster data
        afxByte *indices = NIL;
        afxByte *palette = NIL;
        afxUnit palBpp = meta->palBpp;
        afxUnit palBppAligned = (AFX_ALIGNED_SIZE(meta->palBpp, AFX_BYTE_SIZE));
        afxUnit palPixStride = (AFX_ALIGNED_SIZE(meta->palBpp, AFX_BYTE_SIZE) / AFX_BYTE_SIZE);
        afxUnit wXh = meta->width * meta->height;

        if (AfxAllocate(meta->palSiz * palPixStride, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&palette)) AfxThrowError();
        else
        {
            if (AfxReadStreamAt(in, meta->dataOff, meta->palSiz * palPixStride, 0, palette)) AfxThrowError();
            else
            {
                if (AfxAllocate(wXh, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&indices)) AfxThrowError();
                else
                {
                    if (meta->imgType == 9) // TGA Type 9 refers to a compressed black and white image.
                        DecompressRleChunk(in, meta->width, meta->height, meta->bpp, indices);
                    else if (AfxReadStream(in, wXh * sizeof(afxByte), 0, indices))
                        AfxThrowError();

                    afxUnit i;

                    switch (meta->palBpp)
                    {
                    case 32:
                    {
                        for (i = 0; i < wXh; i++)
                        {
                            data[4 * i + 0] = palette[4 * indices[i] + 0];
                            data[4 * i + 1] = palette[4 * indices[i] + 1];
                            data[4 * i + 2] = palette[4 * indices[i] + 2];
                            data[4 * i + 3] = palette[4 * indices[i] + 3]; // <
                        }
                        break;
                    }
                    case 24:
                    {
                        for (i = 0; i < wXh; i++)
                        {
                            data[3 * i + 0] = palette[3 * indices[i] + 0];
                            data[3 * i + 1] = palette[3 * indices[i] + 1];
                            data[3 * i + 2] = palette[3 * indices[i] + 2];
                        }
                        break;
                    }
                    case 16:
                    {
                        for (i = 0; i < wXh; i++) ((afxUnit16*)data)[i] = ((afxUnit16*)palette)[indices[i]];
                        break;
                    }
                    case 15:
                    {
                        for (i = 0; i < wXh; i++) ((afxUnit16*)data)[i] = ((afxUnit16*)palette)[indices[i]];
                        break;
                    }
                    default: AfxThrowError(); break;
                    }

                    AfxDeallocate((void**)&indices, AfxHere());
                }
            }
            AfxDeallocate((void**)&palette, AfxHere());
        }
        break;
    }
    case 0: // DATA TYPE 0 - PLACEHOLDER, NO DATA RESIDENT.
    {
        break;
    }
    default: AfxThrowError();
    }
    return err;
}
