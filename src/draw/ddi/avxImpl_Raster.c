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

#define _AVX_DRAW_C
#define _AVX_RASTER_C
#include "avxImplementation.h"

_AVXINL void* _AvxGetClientRasterData(avxRaster ras, afxSize from)
{
    return &ras->storage[0].host.bytemap[ras->storageOffset + from];
}

_AVXINL void _AvxDpuUpdateRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxByte const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    afxBool is3d = AvxGetRasterFlags(ras, avxRasterFlag_3D);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &ras->fmt, &pfd);
    afxBool compressed = AvxTestCompressedFormat(ras->fmt);
    afxUnit pixelSize = pfd.stride;

    afxByte* dst = _AvxGetClientRasterData(ras, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        avxRasterRegion const* rgn = &op->rgn;

        avxRasterArrangement lay;
        AvxQueryRasterArrangement(ras, &op->rgn, &lay);
        afxByte* dstData = &dst[lay.offset];

        avxRange mipSize = ras->whd;

        afxByte* dstBase = dstData;
        const afxByte* srcBase = src + op->offset;

        afxUnit copyW = op->rgn.whd.w, copyH = op->rgn.whd.h, copyD = op->rgn.whd.d;

        if (!compressed)
        {
            afxUnit dstRowPitch = mipSize.w * pixelSize;
            afxUnit dstSlicePitch = mipSize.h * dstRowPitch;

            afxUnit srcRowPitch = op->rowStride ? op->rowStride : (copyW * pixelSize);
            afxUnit srcSlicePitch = op->rowsPerImg ? op->rowsPerImg * srcRowPitch : copyH * srcRowPitch;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                size_t dstZOffset = (!is3d)
                    ? (op->rgn.origin.z + z) * dstRowPitch * mipSize.h
                    : (op->rgn.origin.z + z) * dstSlicePitch;

                for (afxUnit y = 0; y < copyH; ++y)
                {
                    const afxByte* src = srcBase + z * srcSlicePitch + y * srcRowPitch;
                    afxByte* dst = dstBase + dstZOffset
                        + (op->rgn.origin.y + y) * dstRowPitch
                        + (op->rgn.origin.x) * pixelSize;

                    AfxCopy(dst, src, copyW * pixelSize);
                }
            }
        }
        else
        {
            afxUnit blockSize = pixelSize;
            afxUnit blocksX = (copyW + 3) / 4;
            afxUnit blocksY = (copyH + 3) / 4;

            afxUnit dstBlocksX = (mipSize.w + 3) / 4;
            afxUnit dstBlocksY = (mipSize.h + 3) / 4;

            afxUnit dstRowPitch = dstBlocksX * blockSize;
            afxUnit dstSlicePitch = dstBlocksY * dstRowPitch;

            afxUnit srcRowPitch = op->rowStride ? op->rowStride : blocksX * blockSize;
            afxUnit srcSlicePitch = op->rowsPerImg ? op->rowsPerImg * srcRowPitch : blocksY * srcRowPitch;

            afxUnit dstXb = op->rgn.origin.x / 4;
            afxUnit dstYb = op->rgn.origin.y / 4;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                size_t dstZOffset = (!is3d)
                    ? (op->rgn.origin.z + z) * dstRowPitch * dstBlocksY
                    : (op->rgn.origin.z + z) * dstSlicePitch;

                for (afxUnit yb = 0; yb < blocksY; ++yb)
                {
                    const afxByte* src = srcBase + z * srcSlicePitch + yb * srcRowPitch;
                    afxByte* dst = dstBase + dstZOffset + (dstYb + yb) * dstRowPitch + dstXb * blockSize;

                    AfxCopy(dst, src, blocksX * blockSize);
                }
            }
        }
    }
}

_AVXINL void _AvxDpuDumpRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxByte* dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    afxBool is3d = AvxGetRasterFlags(ras, avxRasterFlag_3D);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &ras->fmt, &pfd);
    afxBool compressed = AvxTestCompressedFormat(ras->fmt);
    afxUnit pixelSize = pfd.stride;

    afxByte const* src = _AvxGetClientRasterData(ras, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        avxRasterRegion const* rgn = &op->rgn;

        avxRasterArrangement lay;
        AvxQueryRasterArrangement(ras, &op->rgn, &lay);
        afxByte const*  srcData = &src[lay.offset];

        avxRange mipSize = ras->whd;
        for (afxUnit i = 0; i < op->rgn.lodIdx; ++i) {
            mipSize.w = mipSize.w > 1 ? mipSize.w >> 1 : 1;
            mipSize.h = mipSize.h > 1 ? mipSize.h >> 1 : 1;
            mipSize.d = mipSize.d > 1 ? mipSize.d >> 1 : 1;
        }

        const afxByte* srcBase = srcData;
        afxByte* dstBase = dst + op->offset;

        afxUnit copyW = op->rgn.whd.w, copyH = op->rgn.whd.h, copyD = op->rgn.whd.d;

        if (!compressed)
        {
            afxUnit srcRowPitch = mipSize.w * pixelSize;
            afxUnit srcSlicePitch = mipSize.h * srcRowPitch;

            afxUnit dstRowPitch = op->rowStride ? op->rowStride : copyW * pixelSize;
            afxUnit dstSlicePitch = op->rowsPerImg ? op->rowsPerImg * dstRowPitch : copyH * dstRowPitch;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                size_t srcZOffset = (!is3d)
                    ? (op->rgn.origin.z + z) * srcRowPitch * mipSize.h
                    : (op->rgn.origin.z + z) * srcSlicePitch;

                for (afxUnit y = 0; y < copyH; ++y)
                {
                    const afxByte* src = srcBase + srcZOffset
                        + (op->rgn.origin.y + y) * srcRowPitch
                        + (op->rgn.origin.x) * pixelSize;

                    afxByte* dst = dstBase + z * dstSlicePitch + y * dstRowPitch;
                    AfxCopy(dst, src, copyW * pixelSize);
                }
            }
        }
        else
        {
            afxUnit blockSize = pixelSize;
            afxUnit blocksX = (copyW + 3) / 4;
            afxUnit blocksY = (copyH + 3) / 4;

            afxUnit srcBlocksX = (mipSize.w + 3) / 4;
            afxUnit srcBlocksY = (mipSize.h + 3) / 4;

            afxUnit srcRowPitch = srcBlocksX * blockSize;
            afxUnit srcSlicePitch = srcBlocksY * srcRowPitch;

            afxUnit dstRowPitch = op->rowStride ? op->rowStride : blocksX * blockSize;
            afxUnit dstSlicePitch = op->rowsPerImg ? op->rowsPerImg * dstRowPitch  : blocksY * dstRowPitch;

            afxUnit srcXb = op->rgn.origin.x / 4;
            afxUnit srcYb = op->rgn.origin.y / 4;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                size_t srcZOffset = (!is3d)
                    ? (op->rgn.origin.z + z) * srcRowPitch * srcBlocksY
                    : (op->rgn.origin.z + z) * srcSlicePitch;

                for (afxUnit yb = 0; yb < blocksY; ++yb)
                {
                    const afxByte* src = srcBase + srcZOffset + (srcYb + yb) * srcRowPitch + srcXb * blockSize;
                    afxByte* dst = dstBase + z * dstSlicePitch + yb * dstRowPitch;

                    AfxCopy(dst, src, blocksX * blockSize);
                }
            }
        }
    }
}

_AVXINL void _AvxDpuUnpackRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], avxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        avxRasterRegion const* rgn = &op->rgn;

        afxByte const* src = _AvxGetClientBufferData(buf, 0);
        _AvxDpuUpdateRaster(dpu, ras, opCnt, ops, src);
    }
}

_AVXINL void _AvxDpuPackRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], avxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        avxRasterRegion const* rgn = &op->rgn;

        afxByte* dst = _AvxGetClientBufferData(buf, 0);
        _AvxDpuDumpRaster(dpu, ras, opCnt, ops, dst);
    }
}

_AVXINL void _AvxDpuUploadRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxStream in)
{
    afxError err = AFX_ERR_NONE;

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    afxBool is3d = AvxGetRasterFlags(ras, avxRasterFlag_3D);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &ras->fmt, &pfd);
    afxBool compressed = AvxTestCompressedFormat(ras->fmt);
    afxUnit pixelSize = pfd.stride;

    afxByte* dst = _AvxGetClientRasterData(ras, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        avxRasterRegion const* rgn = &op->rgn;

        avxRasterArrangement lay;
        AvxQueryRasterArrangement(ras, &op->rgn, &lay);
        afxByte* dstData = &dst[lay.offset];

        avxRange mipSize = ras->whd;
        for (afxUnit i = 0; i < op->rgn.lodIdx; ++i)
        {
            mipSize.w = mipSize.w > 1 ? mipSize.w >> 1 : 1;
            mipSize.h = mipSize.h > 1 ? mipSize.h >> 1 : 1;
            mipSize.d = mipSize.d > 1 ? mipSize.d >> 1 : 1;
        }

        afxByte* dstBase = dstData;
        afxSize srcBase = /*src +*/ op->offset;

        afxUnit copyW = op->rgn.whd.w, copyH = op->rgn.whd.h, copyD = op->rgn.whd.d;

        if (!compressed)
        {
            afxUnit dstRowPitch = mipSize.w * pixelSize;
            afxUnit dstSlicePitch = mipSize.h * dstRowPitch;

            afxUnit srcRowPitch = op->rowStride ? op->rowStride : copyW * pixelSize;
            afxUnit srcSlicePitch = op->rowsPerImg ? op->rowsPerImg * srcRowPitch : copyH * srcRowPitch;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                size_t dstZOffset = (!is3d)
                    ? (op->rgn.origin.z + z) * dstRowPitch * mipSize.h
                    : (op->rgn.origin.z + z) * dstSlicePitch;

                for (afxUnit y = 0; y < copyH; ++y)
                {
                    afxSize src = srcBase + z * srcSlicePitch + y * srcRowPitch;
                    afxByte* dst = dstBase + dstZOffset
                        + (op->rgn.origin.y + y) * dstRowPitch
                        + (op->rgn.origin.x) * pixelSize;

                    AfxReadStreamAt(in, src, copyW * pixelSize, 0, dst);
                }
            }
        }
        else
        {
            afxUnit blockSize = pixelSize;
            afxUnit blocksX = (copyW + 3) / 4;
            afxUnit blocksY = (copyH + 3) / 4;

            afxUnit dstBlocksX = (mipSize.w + 3) / 4;
            afxUnit dstBlocksY = (mipSize.h + 3) / 4;

            afxUnit dstRowPitch = dstBlocksX * blockSize;
            afxUnit dstSlicePitch = dstBlocksY * dstRowPitch;

            afxUnit srcRowPitch = op->rowStride ? op->rowStride : blocksX * blockSize;
            afxUnit srcSlicePitch = op->rowsPerImg ? op->rowsPerImg : blocksY * srcRowPitch;

            afxUnit dstXb = op->rgn.origin.x / 4;
            afxUnit dstYb = op->rgn.origin.y / 4;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                size_t dstZOffset = (!is3d)
                    ? (op->rgn.origin.z + z) * dstRowPitch * dstBlocksY
                    : (op->rgn.origin.z + z) * dstSlicePitch;

                for (afxUnit yb = 0; yb < blocksY; ++yb)
                {
                    afxSize src = srcBase + z * srcSlicePitch + yb * srcRowPitch;
                    afxByte* dst = dstBase + dstZOffset + (dstYb + yb) * dstRowPitch + dstXb * blockSize;

                    AfxReadStreamAt(in, src, blocksX * pixelSize, 0, dst);
                }
            }
        }
    }
}

_AVX void _AvxDpuDownloadRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    afxBool is3d = AvxGetRasterFlags(ras, avxRasterFlag_3D);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &ras->fmt, &pfd);
    afxBool compressed = AvxTestCompressedFormat(ras->fmt);
    afxUnit pixelSize = pfd.stride;

    afxByte const* src = _AvxGetClientRasterData(ras, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        avxRasterRegion const* rgn = &op->rgn;

        avxRasterArrangement lay;
        AvxQueryRasterArrangement(ras, &op->rgn, &lay);
        afxByte const*  srcData = &src[lay.offset];

        avxRange mipSize = ras->whd;
        for (afxUnit i = 0; i < op->rgn.lodIdx; ++i) {
            mipSize.w = mipSize.w > 1 ? mipSize.w >> 1 : 1;
            mipSize.h = mipSize.h > 1 ? mipSize.h >> 1 : 1;
            mipSize.d = mipSize.d > 1 ? mipSize.d >> 1 : 1;
        }

        const afxByte* srcBase = srcData;
        afxSize dstBase = /*dst +*/ op->offset;

        afxUnit copyW = op->rgn.whd.w, copyH = op->rgn.whd.h, copyD = op->rgn.whd.d;

        if (!compressed)
        {
            afxUnit srcRowPitch = mipSize.w * pixelSize;
            afxUnit srcSlicePitch = mipSize.h * srcRowPitch;

            afxUnit dstRowPitch = op->rowStride ? op->rowStride : copyW * pixelSize;
            afxUnit dstSlicePitch = op->rowsPerImg ? op->rowsPerImg * dstRowPitch : copyH * dstRowPitch;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                size_t srcZOffset = (!is3d)
                    ? (op->rgn.origin.z + z) * srcRowPitch * mipSize.h
                    : (op->rgn.origin.z + z) * srcSlicePitch;

                for (afxUnit y = 0; y < copyH; ++y)
                {
                    const afxByte* src = srcBase + srcZOffset
                        + (op->rgn.origin.y + y) * srcRowPitch
                        + (op->rgn.origin.x) * pixelSize;

                    afxSize dst = dstBase + z * dstSlicePitch + y * dstRowPitch;

                    AfxWriteStreamAt(out, dst, copyW * pixelSize, 0, src);
                }
            }
        }
        else
        {
            afxUnit blockSize = pixelSize;
            afxUnit blocksX = (copyW + 3) / 4;
            afxUnit blocksY = (copyH + 3) / 4;

            afxUnit srcBlocksX = (mipSize.w + 3) / 4;
            afxUnit srcBlocksY = (mipSize.h + 3) / 4;

            afxUnit srcRowPitch = srcBlocksX * blockSize;
            afxUnit srcSlicePitch = srcBlocksY * srcRowPitch;

            afxUnit dstRowPitch = op->rowStride ? op->rowStride : blocksX * blockSize;
            afxUnit dstSlicePitch = op->rowsPerImg ? op->rowsPerImg * dstRowPitch : blocksY * dstRowPitch;

            afxUnit srcXb = op->rgn.origin.x / 4;
            afxUnit srcYb = op->rgn.origin.y / 4;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                size_t srcZOffset = (!is3d)
                    ? (op->rgn.origin.z + z) * srcRowPitch * srcBlocksY
                    : (op->rgn.origin.z + z) * srcSlicePitch;

                for (afxUnit yb = 0; yb < blocksY; ++yb)
                {
                    const afxByte* src = srcBase + srcZOffset + (srcYb + yb) * srcRowPitch + srcXb * blockSize;
                    afxSize dst = dstBase + z * dstSlicePitch + yb * dstRowPitch;
                    AfxWriteStreamAt(out, dst, blocksX * blockSize, 0, src);
                }
            }
        }
    }
}

_AVX void _AvxDpuCopyRaster(avxDpu* dpu, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[], avxRaster src)
{
    afxError err = AFX_ERR_NONE;

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    avxFormatDescription srcPfd, dstPfd;
    AvxDescribeFormats(1, &src->fmt, &srcPfd);
    AvxDescribeFormats(1, &dst->fmt, &dstPfd);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterCopy const* op = &ops[i];

        avxRasterRegion dstRgn;
        dstRgn.whd = op->src.whd;
        dstRgn.origin = op->dstOrigin;
        dstRgn.lodIdx = op->dstLodIdx;
        avxRasterArrangement srcLay, dstLay;
        AvxQueryRasterArrangement(src, &dstRgn, &dstLay);
        AvxQueryRasterArrangement(src, &op->src, &srcLay);

        AFX_ASSERT(op->src.lodIdx < src->mipCnt);
        AFX_ASSERT(op->dstLodIdx < dst->mipCnt);
        AFX_ASSERT(srcPfd.stride == dstPfd.stride);
        AFX_ASSERT(srcPfd.bcWh[0] == dstPfd.bcWh[0]);
        AFX_ASSERT(srcPfd.bcWh[1] == dstPfd.bcWh[1]);

        afxUnit pixelSize = srcPfd.stride;
        afxBool compressed = srcPfd.flags & avxFormatFlag_BC;

        // Compute sizes for selected mip levels
        avxRange srcMipSize = src->whd;
        avxRange dstMipSize = dst->whd;
        for (afxUnit i = 0; i < op->src.lodIdx; ++i)
        {
            srcMipSize.w = srcMipSize.w > 1 ? srcMipSize.w >> 1 : 1;
            srcMipSize.h = srcMipSize.h > 1 ? srcMipSize.h >> 1 : 1;
            srcMipSize.d = srcMipSize.d > 1 ? srcMipSize.d >> 1 : 1;
        }
        for (afxUnit i = 0; i < op->dstLodIdx; ++i)
        {
            dstMipSize.w = dstMipSize.w > 1 ? dstMipSize.w >> 1 : 1;
            dstMipSize.h = dstMipSize.h > 1 ? dstMipSize.h >> 1 : 1;
            dstMipSize.d = dstMipSize.d > 1 ? dstMipSize.d >> 1 : 1;
        }

        const afxByte*srcData = _AvxGetClientRasterData(src, srcLay.offset);
        afxByte*dstData = _AvxGetClientRasterData(dst, dstLay.offset);

        // Base memory pointers for selected LODs
        const afxByte* srcBase = srcData;
        afxByte* dstBase = dstData;

        afxUnit copyW = op->src.whd.w;
        afxUnit copyH = op->src.whd.h;
        afxUnit copyD = op->src.whd.d;

        if (!compressed)
        {
            afxUnit srcRowPitch = srcMipSize.w * pixelSize;
            afxUnit srcSlicePitch = srcMipSize.h * srcRowPitch;

            afxUnit dstRowPitch = dstMipSize.w * pixelSize;
            afxUnit dstSlicePitch = dstMipSize.h * dstRowPitch;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                for (afxUnit y = 0; y < copyH; ++y)
                {
                    const afxByte* src = srcBase
                        + (op->src.origin.z + z) * srcSlicePitch
                        + (op->src.origin.y + y) * srcRowPitch
                        + (op->src.origin.x) * pixelSize;

                    afxByte* dst = dstBase
                        + (op->dstOrigin.z + z) * dstSlicePitch
                        + (op->dstOrigin.y + y) * dstRowPitch
                        + (op->dstOrigin.x) * pixelSize;

                    AfxCopy(dst, src, copyW * pixelSize);
                }
            }
        }
        else
        {
            afxUnit blockW = 4, blockH = 4;
            afxUnit blockSize = pixelSize;

            afxUnit srcBlocksX = (srcMipSize.w + 3) / 4;
            afxUnit srcBlocksY = (srcMipSize.h + 3) / 4;

            afxUnit dstBlocksX = (dstMipSize.w + 3) / 4;
            afxUnit dstBlocksY = (dstMipSize.h + 3) / 4;

            afxUnit copyBlocksX = (copyW + 3) / 4;
            afxUnit copyBlocksY = (copyH + 3) / 4;

            afxUnit srcXb = op->src.origin.x / 4;
            afxUnit srcYb = op->src.origin.y / 4;
            afxUnit dstXb = op->dstOrigin.x / 4;
            afxUnit dstYb = op->dstOrigin.y / 4;

            afxUnit srcRowPitch = srcBlocksX * blockSize;
            afxUnit srcSlicePitch = srcBlocksY * srcRowPitch;

            afxUnit dstRowPitch = dstBlocksX * blockSize;
            afxUnit dstSlicePitch = dstBlocksY * dstRowPitch;

            for (afxUnit z = 0; z < copyD; ++z)
            {
                for (afxUnit yb = 0; yb < copyBlocksY; ++yb)
                {
                    const afxByte* src = srcBase
                        + (op->src.origin.z + z) * srcSlicePitch
                        + (srcYb + yb) * srcRowPitch
                        + (srcXb * blockSize);

                    afxByte* dst = dstBase
                        + (op->dstOrigin.z + z) * dstSlicePitch
                        + (dstYb + yb) * dstRowPitch
                        + (dstXb * blockSize);

                    AfxCopy(dst, src, copyBlocksX * blockSize);
                }
            }
        }
    }
}

_AVX void _AvxDpuResolveRaster(avxDpu* dpu, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[], avxRaster src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    /*
        vkCmdResolveImage
        Purpose: Resolves a multisampled image to a non-multisampled image.
        Filter: Always uses nearest filtering (VK_FILTER_NEAREST).
        Restrictions:
            Source must be multisampled.
            Destination must be single-sampled.
            Format must match.
            Only supports resolving a 1:1 region (no scaling or filtering choice).

        To mimic vkCmdResolveImage, we should:
            Use nearest filtering only (to match Vulkan's behavior).
            Ensure that we resolve from a multisample framebuffer into a single-sample texture.
            Avoid any format conversion or scaling.
    */
    AFX_ASSERT(src->mipCnt > 1);
    AFX_ASSERT(AvxGetRasterFlags(src, avxRasterFlag_MULTISAMP));
    AFX_ASSERT(src->mipCnt == 1);
    //AFX_ASSERT(!AvxGetRasterFlags(dst, avxRasterFlag_MULTISAMP));
    //AFX_ASSERT(src->m.fmt == dst->m.fmt); // GL can deal with some potential convertions.

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterCopy const* op = &ops[i];

        for (afxUnit srcZ = 0, dstZ = 0; (srcZ < op->src.whd.d) && (dstZ < op->src.whd.d); srcZ++, dstZ++)
        {
            //gl->BlitFramebuffer(op->src.origin.x, op->src.origin.y, op->src.whd.w, op->src.whd.h, op->dstOrigin.x, op->dstOrigin.y, op->src.whd.w, op->src.whd.h, blitMask, filter); _ZglThrowErrorOccuried();
        }
    }
}

// Compute linear offset into data for pixel at x,y,z in mip
static afxSize avxComputePixelOffset(
    afxUnit x, afxUnit y, afxUnit z,
    avxRange mipSize,
    afxUnit pixelStride)
{
    return ((afxSize)z * mipSize.h * mipSize.w + (afxSize)y * mipSize.w + (afxSize)x) * pixelStride;
}

// Stub: Calculate offset of mip level in linear data
// Here assuming mipmaps are stored sequentially in memory; adjust if not.
static afxSize avxComputeMipOffset(avxRaster raster, afxUnit lod) {
    afxSize offset = 0;
    avxRange size = raster->whd;
    for (afxUnit i = 0; i < lod; ++i) {
        size.w = size.w >> 1; if (size.w == 0) size.w = 1;
        size.h = size.h >> 1; if (size.h == 0) size.h = 1;
        size.d = size.d >> 1; if (size.d == 0) size.d = 1;
        offset += (afxSize)size.w * size.h * size.w * raster->fmtStride;
    }
    return offset;
}

// Format-aware readPixel: reads one texel components into float[4]
static void readPixel(
    const afxByte* base,
    afxUnit pixelStride,
    avxRange mipSize,
    const avxFormatDescription* fmtDesc,
    afxUnit x, afxUnit y, afxUnit z,
    float outComp[4])
{
    // Clamp coords for safety (redundant if caller clamps)
    x = AFX_CLAMP(x, 0, mipSize.w - 1);
    y = AFX_CLAMP(y, 0, mipSize.h - 1);
    z = AFX_CLAMP(z, 0, mipSize.d - 1);

    const afxByte* pixelPtr = base + avxComputePixelOffset(x, y, z, mipSize, pixelStride);

    afxUnit bitOffset = 0;
    for (afxUnit c = 0; c < fmtDesc->compCnt; ++c)
    {
        const avxFormatType compType = fmtDesc->type[c];
        afxUnit bpc = fmtDesc->bpc[c];
        afxUnit byteOffset = bitOffset / 8;
        const afxByte* compPtr = pixelPtr + byteOffset;

        // Read component raw value depending on type and size:
        float val = 0.f;
        switch (compType)
        {
        case avxFormatType_UN:
        case avxFormatType_U:
            if (bpc == 8)      val = (float)(*(const uint8_t*)compPtr);
            else if (bpc == 16) val = (float)(*(const uint16_t*)compPtr);
            else if (bpc == 32) val = (float)(*(const uint32_t*)compPtr);
            else val = 0.f; // unsupported
            // Scale by tof if normalized (assuming tof[] is 1/ (max int))
            if (fmtDesc->isNormalized[c])
                val *= fmtDesc->tof[c];
            break;

        case avxFormatType_SN:
        case avxFormatType_I:
            if (bpc == 8)      val = (float)(*(const int8_t*)compPtr);
            else if (bpc == 16) val = (float)(*(const int16_t*)compPtr);
            else if (bpc == 32) val = (float)(*(const int32_t*)compPtr);
            else val = 0.f;
            if (fmtDesc->isNormalized[c])
                val *= fmtDesc->tof[c];
            break;

        case avxFormatType_F:
            if (bpc == 32)     val = *(const float*)compPtr;
            else if (bpc == 64) val = (float)(*(const double*)compPtr);
            else val = 0.f;
            break;

        default:
            val = 0.f; // unsupported type
            break;
        }

        outComp[c] = val;
        bitOffset += bpc;
    }

    // Fill missing components with defaults
    for (afxUnit c = fmtDesc->compCnt; c < 4; ++c) {
        outComp[c] = (float)fmtDesc->defaults[c];
    }
}

// Format-aware writePixel: writes float[4] components into one texel
static void writePixel(
    afxByte* base,
    afxUnit pixelStride,
    avxRange mipSize,
    const avxFormatDescription* fmtDesc,
    afxUnit x, afxUnit y, afxUnit z,
    const float inComp[4])
{
    // Clamp coords
    x = AFX_CLAMP(x, 0, mipSize.w - 1);
    y = AFX_CLAMP(y, 0, mipSize.h - 1);
    z = AFX_CLAMP(z, 0, mipSize.d - 1);

    afxByte* pixelPtr = base + avxComputePixelOffset(x, y, z, mipSize, pixelStride);

    afxUnit bitOffset = 0;
    for (afxUnit c = 0; c < fmtDesc->compCnt; ++c)
    {
        avxFormatType compType = fmtDesc->type[c];
        afxUnit bpc = fmtDesc->bpc[c];
        afxUnit byteOffset = bitOffset / 8;
        afxByte* compPtr = pixelPtr + byteOffset;

        float val = inComp[c];

        // Clamp float val for normalized types
        if (fmtDesc->isNormalized[c]) {
            if (compType == avxFormatType_UN || compType == avxFormatType_U)
                val = AFX_CLAMP(val, 0.f, 1.f);
            else if (compType == avxFormatType_SN || compType == avxFormatType_I)
                val = AFX_CLAMP(val, -1.f, 1.f);
        }

        // Write component raw value depending on type and size
        switch (compType)
        {
        case avxFormatType_UN:
        case avxFormatType_U:
            if (fmtDesc->isNormalized[c])
                val /= fmtDesc->tof[c]; // unscale to int range

            if (bpc == 8)      *(uint8_t*)compPtr = (uint8_t)(val + 0.5f);
            else if (bpc == 16) *(uint16_t*)compPtr = (uint16_t)(val + 0.5f);
            else if (bpc == 32) *(uint32_t*)compPtr = (uint32_t)(val + 0.5f);
            break;

        case avxFormatType_SN:
        case avxFormatType_I:
            if (fmtDesc->isNormalized[c])
                val /= fmtDesc->tof[c];

            if (bpc == 8)      *(int8_t*)compPtr = (int8_t)(val + (val >= 0 ? 0.5f : -0.5f));
            else if (bpc == 16) *(int16_t*)compPtr = (int16_t)(val + (val >= 0 ? 0.5f : -0.5f));
            else if (bpc == 32) *(int32_t*)compPtr = (int32_t)(val + (val >= 0 ? 0.5f : -0.5f));
            break;

        case avxFormatType_F:
            if (bpc == 32)     *(float*)compPtr = val;
            else if (bpc == 64) *(double*)compPtr = (double)val;
            break;

        default:
            // Unsupported
            break;
        }

        bitOffset += bpc;
    }
}

// Trilinear interpolation sample of source texture at float coords
static void trilinearSample(
    const afxByte* base,
    afxUnit pixelStride,
    avxRange mipSize,
    const avxFormatDescription* fmtDesc,
    float sx, float sy, float sz,
    float outComp[4])
{
    // Calculate integer base coordinates and fractional parts
    int x0 = (int)floorf(sx);
    int y0 = (int)floorf(sy);
    int z0 = (int)floorf(sz);

    float fx = sx - x0;
    float fy = sy - y0;
    float fz = sz - z0;

    // Clamp base coords inside valid range for sampling neighbors
    x0 = AFX_CLAMP(x0, 0, (afxUnit)(mipSize.w - 1));
    y0 = AFX_CLAMP(y0, 0, (afxUnit)(mipSize.h - 1));
    z0 = AFX_CLAMP(z0, 0, (afxUnit)(mipSize.d - 1));

    // Compute neighbor coordinates, clamped
    int x1 = AFX_CLAMP(x0 + 1, 0, (afxUnit)(mipSize.w - 1));
    int y1 = AFX_CLAMP(y0 + 1, 0, (afxUnit)(mipSize.h - 1));
    int z1 = AFX_CLAMP(z0 + 1, 0, (afxUnit)(mipSize.d - 1));

    float c000[4], c100[4], c010[4], c110[4];
    float c001[4], c101[4], c011[4], c111[4];

    readPixel(base, pixelStride, mipSize, fmtDesc, x0, y0, z0, c000);
    readPixel(base, pixelStride, mipSize, fmtDesc, x1, y0, z0, c100);
    readPixel(base, pixelStride, mipSize, fmtDesc, x0, y1, z0, c010);
    readPixel(base, pixelStride, mipSize, fmtDesc, x1, y1, z0, c110);
    readPixel(base, pixelStride, mipSize, fmtDesc, x0, y0, z1, c001);
    readPixel(base, pixelStride, mipSize, fmtDesc, x1, y0, z1, c101);
    readPixel(base, pixelStride, mipSize, fmtDesc, x0, y1, z1, c011);
    readPixel(base, pixelStride, mipSize, fmtDesc, x1, y1, z1, c111);

    for (int i = 0; i < 4; ++i) {
        float c00 = c000[i] * (1 - fx) + c100[i] * fx;
        float c01 = c001[i] * (1 - fx) + c101[i] * fx;
        float c10 = c010[i] * (1 - fx) + c110[i] * fx;
        float c11 = c011[i] * (1 - fx) + c111[i] * fx;

        float c0 = c00 * (1 - fy) + c10 * fy;
        float c1 = c01 * (1 - fy) + c11 * fy;

        outComp[i] = c0 * (1 - fz) + c1 * fz;
    }
}

_AVX void _AvxDpuBlitRaster(avxDpu* dpu, avxRaster dst, afxUnit opCnt, avxRasterBlit const ops[], avxRaster src, avxTexelFilter flt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    /*
        Core ideas

        Compute mapping: For each destination texel, find corresponding source coordinate.
        For NEAREST filter: round to nearest integer coordinate in source.
        For LINEAR filter: perform trilinear interpolation of up to 8 neighboring source texels.
        Convert components to float, interpolate, then convert back.
        Use bpc[], type[] to read/write components safely.
        Support layered and volumetric textures via avxRasterRegion.origin.z and .whd.z
        Handle mipmap levels via .lodIdx
        Assume avxRaster->data layout is linear with proper pixelStride and depth slices
    */

    avxFormatDescription fmtDesc;
    AvxDescribeFormats(1, &dst->fmt, &fmtDesc);

    const afxByte* srcData = _AvxGetClientRasterData(src, 0);
    afxByte* dstData = _AvxGetClientRasterData(dst, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterBlit const* op = &ops[i];

        avxRasterArrangement srcLay, dstLay;
        AvxQueryRasterArrangement(src, &op->src, &srcLay);
        AvxQueryRasterArrangement(dst, &op->dst, &dstLay);

        // Compute mip sizes for src and dst
        avxRange srcMipSize = op->src.whd;
        avxRange dstMipSize = op->dst.whd;

        // Prepare base pointers for src and dst mip levels
        const afxByte* srcBase = srcData + srcLay.offset;
        afxByte* dstBase = dstData + dstLay.offset;

        // Calculate scale factors for coordinate mapping:
        // srcCoord = srcOrigin + (dstCoord - dstOrigin) * (srcSize / dstSize)
        afxReal scaleX = (afxReal)op->src.whd.w / (afxReal)op->dst.whd.w;
        afxReal scaleY = (afxReal)op->src.whd.h / (afxReal)op->dst.whd.h;
        afxReal scaleZ = (afxReal)op->src.whd.d / (afxReal)op->dst.whd.d;

        if (flt == avxTexelFilter_NEAREST)
        {
#pragma omp parallel for collapse(3)
            for (afxUnit z = 0; z < op->dst.whd.d; ++z)
            {
                for (afxUnit y = 0; y < op->dst.whd.h; ++y)
                {
                    for (afxUnit x = 0; x < op->dst.whd.w; ++x)
                    {
                        afxUnit srcX = (afxUnit)(op->src.origin.x + (x + 0.5f) * scaleX);
                        afxUnit srcY = (afxUnit)(op->src.origin.y + (y + 0.5f) * scaleY);
                        afxUnit srcZ = (afxUnit)(op->src.origin.z + (z + 0.5f) * scaleZ);

                        srcX = AFX_CLAMP(srcX, 0, srcMipSize.w - 1);
                        srcY = AFX_CLAMP(srcY, 0, srcMipSize.h - 1);
                        srcZ = AFX_CLAMP(srcZ, 0, srcMipSize.d - 1);

                        float pixelComp[4];
                        readPixel(srcBase, src->fmtStride, srcMipSize, &fmtDesc, srcX, srcY, srcZ, pixelComp);

                        afxUnit dstX = op->dst.origin.x + x;
                        afxUnit dstY = op->dst.origin.y + y;
                        afxUnit dstZ = op->dst.origin.z + z;

                        writePixel(dstBase, dst->fmtStride, dstMipSize, &fmtDesc, dstX, dstY, dstZ, pixelComp);
                    }
                }
            }
        }
        else
        {
            // For each dst z,y,x within blit->dst.whd
            for (afxUnit z = 0; z < op->dst.whd.d; ++z)
            {
                for (afxUnit y = 0; y < op->dst.whd.h; ++y)
                {
                    for (afxUnit x = 0; x < op->dst.whd.w; ++x)
                    {
                        // Compute src coordinates in float
                        afxReal srcXf = op->src.origin.x + (x + 0.5f) * scaleX;
                        afxReal srcYf = op->src.origin.y + (y + 0.5f) * scaleY;
                        afxReal srcZf = op->src.origin.z + (z + 0.5f) * scaleZ;

                        float pixelComp[4] = { 0 };
                        // Trilinear interpolation here; read up to 8 samples, interpolate, write to pixelComp
                        trilinearSample(srcBase, src->fmtStride, srcMipSize, &fmtDesc, srcXf, srcYf, srcZf, pixelComp);

                        // Write pixelComp to dst at (x,y,z)
                        writePixel(dstBase, dst->fmtStride, dstMipSize, &fmtDesc, op->dst.origin.x + x, op->dst.origin.y + y, op->dst.origin.z + z, pixelComp);
                    }
                }
            }
        }
    }
}
#include <stdint.h>

uint16_t float32_to_float16(float value)
{
    union {
        float f;
        uint32_t u;
    } in;

    in.f = value;

    uint32_t f = in.u;
    uint32_t sign = (f >> 16) & 0x8000;
    uint32_t mantissa = f & 0x007FFFFF;
    int32_t exp = ((f >> 23) & 0xFF) - 127 + 15;

    if (exp <= 0) {
        // Denormal or zero
        if (exp < -10)
            return (uint16_t)sign; // too small, becomes zero

        // Normalize
        mantissa |= 0x00800000;
        uint32_t t = 14 - exp;
        uint32_t m = mantissa >> t;
        return (uint16_t)(sign | m);
    }
    else if (exp == 0xFF - 127 + 15) {
        // Inf or NaN
        if (mantissa == 0) {
            return (uint16_t)(sign | 0x7C00); // Inf
        }
        else {
            return (uint16_t)(sign | 0x7C00 | (mantissa >> 13)); // NaN
        }
    }
    else if (exp >= 31) {
        // Overflow => Inf
        return (uint16_t)(sign | 0x7C00);
    }
    else {
        // Normalized number
        return (uint16_t)(sign | (exp << 10) | (mantissa >> 13));
    }
}

void packClearPixel(
    afxByte* out,
    afxUnit pixelStride,
    const avxFormatDescription* fmtDesc,
    const avxClearValue* clearVal)
{
    // helper function that serialize the avxClearValue into a pixel-aligned memory buffer, respecting format type and layout.

    assert(out && fmtDesc);

    AfxFill(out, 0, pixelStride); // zero-fill in case of padding

    for (afxUnit i = 0; i < fmtDesc->compCnt; ++i) {
        afxByte* dst = out + fmtDesc->bpc[i] / 8 * i;

        switch (fmtDesc->type[i]) {
        case avxFormatType_UN:
        case avxFormatType_US:
        case avxFormatType_U:
            switch (fmtDesc->bpc[i]) {
            case 8:
                *(uint8_t*)dst = (uint8_t)clearVal->rgban[i];
                break;
            case 16:
                *(uint16_t*)dst = (uint16_t)clearVal->rgban[i];
                break;
            case 32:
                *(uint32_t*)dst = (uint32_t)clearVal->rgban[i];
                break;
            }
            break;

        case avxFormatType_SN:
        case avxFormatType_SS:
        case avxFormatType_I:
            switch (fmtDesc->bpc[i]) {
            case 8:
                *(int8_t*)dst = (int8_t)clearVal->rgbai[i];
                break;
            case 16:
                *(int16_t*)dst = (int16_t)clearVal->rgbai[i];
                break;
            case 32:
                *(int32_t*)dst = (int32_t)clearVal->rgbai[i];
                break;
            }
            break;

        case avxFormatType_F:
            switch (fmtDesc->bpc[i]) {
            case 16:
                *(uint16_t*)dst = float32_to_float16(clearVal->rgba[i]); // you'd need a float16 conversion function
                break;
            case 32:
                *(float*)dst = (float)clearVal->rgba[i];
                break;
            }
            break;

        default:
            break;
        }
    }
}

void packClearDepthStencil(
    afxByte* out,
    afxUnit pixelStride,
    const avxFormatDescription* fmtDesc,
    const avxClearValue* clearVal)
{
    // helper functions that serialize the avxClearValue into a pixel-aligned memory buffer, respecting format type and layout.

    assert(out && fmtDesc);
    AfxFill(out, 0, pixelStride); // zero-fill

    if ((fmtDesc->flags & avxFormatFlag_DEPTH) && (fmtDesc->flags & avxFormatFlag_STENCIL)) {
        // Combined depth-stencil (e.g., D24S8, D32S8)
        if (fmtDesc->bpp == 32) {
            // Example: D24S8 packed in 32-bit
            uint32_t packed = ((uint32_t)(clearVal->depth * 0xFFFFFF) & 0xFFFFFF) |
                ((clearVal->stencil & 0xFF) << 24);
            AfxCopy(out, &packed, 4);
        }
        else if (fmtDesc->bpp == 64) {
            // Example: D32F_S8
            float depth = clearVal->depth;
            uint8_t stencil = (uint8_t)clearVal->stencil;
            AfxCopy(out, &depth, 4);
            AfxCopy(out + 4, &stencil, 1);
        }
    }
    else if (fmtDesc->flags & avxFormatFlag_DEPTH) {
        if (fmtDesc->bpc[0] == 32 && fmtDesc->type[0] == avxFormatType_F) {
            float depth = clearVal->depth;
            AfxCopy(out, &depth, 4);
        }
        else if (fmtDesc->bpc[0] == 16) {
            uint16_t depth16 = float32_to_float16(clearVal->depth);
            AfxCopy(out, &depth16, 2);
        }
    }
    else if (fmtDesc->flags & avxFormatFlag_STENCIL) {
        uint8_t stencil = (uint8_t)clearVal->stencil;
        AfxCopy(out, &stencil, 1);
    }
}

_AVX void _AvxDpuClearRaster(avxDpu* dpu, avxRaster ras, avxClearValue const* clearVal, afxUnit baseLodIdx, afxUnit lodCnt, afxUnit baseLayer, afxUnit layerCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxByte* rasData = _AvxGetClientRasterData(ras, 0);
    afxUnit pixelStride = ras->fmtStride;

    avxFormatDescription fmtDesc;
    AvxDescribeFormats(1, &ras->fmt, &fmtDesc);

    avxRasterRegion rgn;

    for (afxUnit lod = 0; lod < baseLodIdx; ++lod)
    {
        rgn.lodIdx = baseLodIdx + lod;
        avxRange mipSize = AvxGetRasterExtent(ras, rgn.lodIdx);
        rgn.whd = mipSize;
        rgn.whd.d = 1;

        for (afxUnit layer = 0; layer < layerCnt; ++layer)
        {
            rgn.origin.z = baseLayer + layer;
            avxRasterArrangement lay;
            AvxQueryRasterArrangement(ras, &rgn, &lay);
            afxByte* layerData = &rasData[lay.offset];

            if (fmtDesc.flags & avxFormatFlag_COLOR)
            {
                // Clear color buffer
                // Prepare clear pixel data buffer matching pixelStride
                afxByte clearPixel[64]; // enough for max pixelStride (adjust if needed)
                assert(pixelStride <= sizeof(clearPixel));

                // Pack clearVal->rgba or clearVal->rgban into clearPixel according to fmtDesc
                packClearPixel(clearPixel, pixelStride, &fmtDesc, clearVal);

                // Fill whole layer slice with clearPixel repeated
                for (afxUnit y = 0; y < mipSize.h; ++y)
                {
                    afxByte* row = layerData + y * mipSize.w * pixelStride;
                    for (afxUnit x = 0; x < mipSize.w; ++x)
                    {
                        AfxCopy(row + x * pixelStride, clearPixel, pixelStride);
                    }
                }
            }
            else if ((fmtDesc.flags & avxFormatFlag_DEPTH) || (fmtDesc.flags & avxFormatFlag_STENCIL))
            {
                // Depth/stencil clear

                // Assume depth and stencil packed as floats and uints, or packed formats (like D24S8)
                // Simplify: treat as array of pixelStride bytes per texel and fill by repeating packed clear value

                afxByte clearPixel[64]; // again, max pixelStride buffer
                AFX_ASSERT(pixelStride <= sizeof(clearPixel));

                packClearDepthStencil(clearPixel, pixelStride, &fmtDesc, clearVal);

                for (afxUnit y = 0; y < mipSize.h; ++y)
                {
                    afxByte* row = layerData + y * mipSize.w * pixelStride;
                    for (afxUnit x = 0; x < mipSize.w; ++x)
                    {
                        AfxCopy(row + x * pixelStride, clearPixel, pixelStride);
                    }
                }
            }
            else
            {
                // Unsupported format, do nothing or zero clear
                AfxFill(layerData, 0, mipSize.w * mipSize.h * pixelStride);
            }
        }
    }
}
