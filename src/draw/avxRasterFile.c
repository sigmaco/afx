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

// This software is part of Advanced Video Graphics Extensions & Experiments.

#include "avxIcd.h"

static_assert(sizeof(avxFormat) == sizeof(afxUnit32), "");
static_assert(sizeof(avxTgaFlags) == sizeof(afxUnit32), "");
static_assert(sizeof(avxTgaCodec) == sizeof(afxUnit32), "");

#if 0
void LoadRLERow(avxRasterFile* tga, afxStream kIst, afxByte* dst, afxUnit uiDestSize)
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
    afxError err = { 0 };
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
        if (AfxReadStream(stream, sizeof(afxByte), 0, &hdr))
            break;

        afxUnit runLen = (hdr & 0x7F) + 1;

        if (hdr & 0x80) // high bit is set when it's a run-length packet.
        {
            afxByte buf[4];
            if (AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, buf))
                break;

            for (afxUnit i = 0; i < runLen; i++)
            {
                AfxCopy(&dst[currByte], buf, byteCnt);
                currByte += byteCnt;
            }
        }
        else
        {
            // Raw packet

            for (afxUnit i = 0; i < runLen; i++)
            {
                if (AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, &dst[currByte]))
                    break;

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

_AVX afxError AvxPrepareRasterFile(avxRasterFile* tga, avxRasterIo* iop, afxUnit lodCnt, avxFormat fmt, avxRasterFlags flags, afxUnit uddSiz)
{
    afxError err = { 0 };
    AFX_ASSERT(tga);
    AFX_ASSERT(iop);
    AFX_ASSERT(fmt);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);

    // begin file
    // record the TGA 2000 header

    /*
        Calculate Stride (Row Size) and Row Count
        For a compressed texture, the stride (the length of each row in memory) is calculated
        based on the width of the texture and the size of the compressed blocks.

        Stride (row size) = (width of the texture / 4) * bytes per block

        For DXT1: (width / 4) * 8
        For DXT5: (width / 4) * 16
        Row count (number of rows) = height / 4 (rounded up if the height isn't a multiple of 4).
    */

    afxUnit uddSiz2 = AFX_ALIGN_SIZE(uddSiz, AFX_SIMD_ALIGNMENT);
    afxUnit w = AFX_MAX(pfd.bcWh[0], AFX_ALIGN_SIZE(iop->rgn.whd.w, pfd.bcWh[0]));
    afxUnit h = AFX_MAX(pfd.bcWh[1], AFX_ALIGN_SIZE(iop->rgn.whd.h, pfd.bcWh[1]));
    iop->rgn.whd.w = w;
    iop->rgn.whd.h = h;
    iop->rgn.whd.d = AFX_MAX(1, iop->rgn.whd.d);
    iop->rgn.origin.x = AFX_ALIGN_SIZE(iop->rgn.origin.x, pfd.bcWh[0]);
    iop->rgn.origin.y = AFX_ALIGN_SIZE(iop->rgn.origin.y, pfd.bcWh[1]);
    iop->rgn.origin.z = iop->rgn.origin.z;

    // ignored for block compressed types.
    iop->rowStride = iop->rowStride ? iop->rowStride : AFX_ALIGN_SIZE((w / pfd.bcWh[0]) * pfd.stride, AFX_SIMD_ALIGNMENT); // align
    iop->rowsPerImg = iop->rowsPerImg ? iop->rowsPerImg : ((h + (pfd.bcWh[1] - 1)) / pfd.bcWh[1]); // round up
    iop->decSiz = iop->rowStride * iop->rowsPerImg;
    iop->encSiz = iop->rowStride * iop->rowsPerImg;

    avxRasterFile tgai = { 0 };
    tgai.idd.fcc[0] = 't';
    tgai.idd.fcc[1] = 'g';
    tgai.idd.fcc[2] = 'a';
    tgai.idd.fcc[3] = '4';
    tgai.extLen = (AFX_TGA_EXT_SIZ)+uddSiz2;
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
    tgai.depth = iop->rgn.whd.d;
    tgai.lodCnt = AFX_MAX(1, lodCnt);
    tgai.baseLod = iop->rgn.lodIdx;
    tgai.fmt = fmt ? fmt : avxFormat_BGRA8un;
    tgai.bpp = pfd.bpp / (pfd.bcWh[0] * pfd.bcWh[1]); // 128 / (4*4) == 8, 32 / 1*1 ==32
    tgai.descriptor = 0;

    if (pfd.bpc[3])
        tgai.descriptor |= (AFX_MIN(pfd.bpc[3], 8) & 0x0F); // Set to 8 alpha bits --- (8 & 0x0F)
#if 0
    if (flags & avxRasterFlag_MIRRORED)
        tgai.flags |= avxTgaFlag_MIRRORED;

    if (flags & avxRasterFlag_FLIPPED)
        tgai.flags |= avxTgaFlag_FLIPPED;
#endif
    if (flags & avxRasterFlag_CUBEMAP)
        tgai.flags |= avxTgaFlag_CUBEMAP;

    if (flags & avxRasterFlag_3D)
        tgai.flags |= avxTgaFlag_3D;

    // ignored for block compressed types.
    tgai.rowStride = iop->rowStride;
    tgai.rowsPerImg = iop->rowsPerImg;
    tgai.decSiz = iop->decSiz; // tgai.rowStride * tgai.rowsPerImg;
    tgai.encSiz = iop->encSiz; // tgai.decSiz;
    tgai.codec = 0;// iop->codec; // 0;

    tgai.dataOff = iop->offset + sizeof(avxRasterFile) + uddSiz2;
    
    *tga = tgai;

    return err;
}

_AVX afxError AvxWriteRasterFile(avxRasterFile* tgai, void* udd, afxUnit uddSiz, afxStream out)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(!uddSiz || udd);
    AFX_ASSERT(tgai);

    afxSize posn = AfxAskStreamPosn(out);
    tgai->dataOff += posn;

    if (AfxWriteStream(out, sizeof(avxRasterFile), 0, tgai))
        AfxThrowError();

    afxUnit uddSiz2 = AFX_ALIGN_SIZE(uddSiz, AFX_SIMD_ALIGNMENT);
    AFX_ASSERT(AfxAskStreamPosn(out) == 64 + uddSiz2);

    if (uddSiz)
    {
        AFX_ASSERT(udd);

        if (AfxWriteStream(out, uddSiz, 0, udd))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AvxReadRasterFile(avxRasterFile* info, afxStream in)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(info);
    afxUnit i;
    
    afxBool legacy = TRUE;

    avxRasterFile tgai;
    if (AfxReadStream(in, AFX_TGA_HDR_SIZ, 0, &tgai))
    {
        AfxThrowError();
        return err;
    }

    if (tgai.extLen >= AFX_TGA_EXT_SIZ)
    {
        if (AfxReadStream(in, AFX_TGA_EXT_SIZ, 0, &(((afxByte*)&tgai)[AFX_TGA_HDR_SIZ])))
        {
            AfxThrowError();
            return err;
        }

        if ((tgai.idd.fcc[0] != 't') || (tgai.idd.fcc[1] != 'g') || (tgai.idd.fcc[2] != 'a') || (tgai.idd.fcc[3] != '4'))
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
            info->encSiz = tgai.encSiz;
            info->decSiz = tgai.decSiz;
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

        info->dataOff = AfxAskStreamPosn(in);
        info->decSiz = 0;
        info->encSiz = 0;
        info->codec = (tgai.imgType >= 9 && 11 <= tgai.imgType) ? avxTgaCodec_RLE8 : avxTgaCodec_NONE;

        // "Attribute" bits per pixel (usually alpha bits).
        // Check both pixel depth and image descriptor to confirm if alpha is present.
        afxUnit8 alphaBits = (tgai.descriptor & 0x0F);
        
        // If bit 5 is not set (0), image is stored bottom-up (not inverted).
        // If bit 5 is set (1), image is stored top-down  (inverted vertically, common for modern usage).
        // This is often where people get confused. Bit 5 is a vertical flip flag.
        afxBool isTopdown = (tgai.descriptor & AFX_BITMASK(5));
        if (isTopdown) tgai.flags |= avxTgaFlag_FLIPPED;

        // Bit 4: Left-to-right (horizontal origin).
        // If bit is not set (0), origin is on left side (standard).
        // If bit is set (1), origin is on right side (mirrored horizontally).
        afxBool isRightToLeft = (tgai.descriptor & AFX_BITMASK(4));
        if (isRightToLeft) tgai.flags |= avxTgaFlag_MIRRORED;

        afxUnit8 reservedBits = tgai.descriptor >> 6; // bits 6–7

        switch (tgai.imgType)
        {
        case 2: // DATA TYPE 2 - TRUE-COLOR IMAGES
        case 10: // DATA TYPE 10 - RUN-LENGTH ENCODED (RLE), TRUE-COLOR IMAGES
        {
            // packed raster data
#if 0
            afxUnit bppAligned = (AFX_ALIGN_SIZE(tgai.bpp, AFX_BYTE_SIZE));
            afxUnit pixStride = (AFX_ALIGN_SIZE(bppAligned, AFX_BYTE_SIZE) / AFX_BYTE_SIZE);
            afxUnit rowStride = info->width * pixStride;

            info->rowStride = rowStride;
            info->rowsPerImg = info->height;
            info->decSiz = info->rowsPerImg * AFX_ALIGN_SIZE(info->rowStride, AFX_SIMD_ALIGNMENT);
            info->encSiz = info->rowsPerImg * info->rowStride;
#endif       
            switch (tgai.bpp)
            {
            case 32: info->fmt = avxFormat_BGRA8un; break;
            case 24: info->fmt = avxFormat_BGR8un; break;
            case 16: info->fmt = avxFormat_BGRA4un; break;
            case 15: info->fmt = avxFormat_BGR5A1un; break;
            case 12: info->fmt = avxFormat_A2BGR10un; break;
            default: AfxThrowError(); break;
            }

            break;
        }
        case 3: // DATA TYPE 3 - BLACK AND WHITE (UNMAPPED) IMAGES
        case 11: // DATA TYPE 11 - RUN-LENGTH ENCODED (RLE), BLACK AND WHITE IMAGES
        {
            // packed raster data
#if 0
            afxUnit bppAligned = (AFX_ALIGN_SIZE(tgai.bpp, AFX_BYTE_SIZE));
            afxUnit pixStride = (AFX_ALIGN_SIZE(bppAligned, AFX_BYTE_SIZE) / AFX_BYTE_SIZE);
            afxUnit rowStride = info->width * pixStride;

            info->rowStride = rowStride;
            info->rowsPerImg = info->height;
            info->decSiz = info->rowsPerImg * AFX_ALIGN_SIZE(info->rowStride, AFX_SIMD_ALIGNMENT);
            info->encSiz = info->rowsPerImg * info->rowStride;
#endif       
            switch (tgai.bpp)
            {
            case  8: info->fmt = avxFormat_R8un; break;
            case 16: info->fmt = avxFormat_R16un; break;
            case 24: info->fmt = avxFormat_X8D24un; break;
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
#if 0
            afxUnit bppAligned = (AFX_ALIGN_SIZE(tgai.palBpp, AFX_BYTE_SIZE));
            afxUnit pixStride = (AFX_ALIGN_SIZE(bppAligned, AFX_BYTE_SIZE) / AFX_BYTE_SIZE);
            //info->rowStride = info->width * pixStride;
            //info->imgStride = info->height * info->rowStride;
            //info->decSiz = info->height * info->rowStride;
            //info->encSiz = info->height * info->rowStride;
#endif
            switch (tgai.palBpp)
            {
            case 32: info->fmt = avxFormat_BGRA8un; break;
            case 24: info->fmt = avxFormat_BGR8un; break;
            case 16: info->fmt = avxFormat_BGRA4un; break;
            case 15: info->fmt = avxFormat_BGR5A1un; break;
            case 12: info->fmt = avxFormat_A2BGR10un; break;
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

        /*
            Calculate Stride (Row Size) and Row Count
            For a compressed texture, the stride (the length of each row in memory) is calculated
            based on the width of the texture and the size of the compressed blocks.

            Stride (row size) = (width of the texture / 4) * bytes per block

            For DXT1: (width / 4) * 8
            For DXT5: (width / 4) * 16
            Row count (number of rows) = height / 4 (rounded up if the height isn't a multiple of 4).
        */

        avxFormatDescription pfd;
        AvxDescribeFormats(1, (avxFormat[]) { info->fmt }, &pfd);
        info->rowStride = (tgai.width / pfd.bcWh[0]) * pfd.stride;
        info->rowsPerImg = (tgai.height + (pfd.bcWh[1] - 1)) / pfd.bcWh[1];
        info->decSiz = info->rowsPerImg * info->rowStride;
        info->encSiz = info->rowsPerImg * info->rowStride;

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

_AVX afxError AvxDecodeRasterFile(avxRasterFile const* meta, afxStream in, void* dst)
{
    afxError err = { 0 };
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
        afxUnit palBppAligned = (AFX_ALIGN_SIZE(meta->palBpp, AFX_BYTE_SIZE));
        afxUnit palPixStride = (AFX_ALIGN_SIZE(meta->palBpp, AFX_BYTE_SIZE) / AFX_BYTE_SIZE);
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
