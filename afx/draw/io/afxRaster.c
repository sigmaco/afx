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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_RASTER_C
#include "../../dev/AvxImplKit.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x whd >> lod

AVX afxError _AfxPrintRasterToTarga(afxRaster ras, afxRasterIo const* iop, afxNat lodCnt, afxStream out, afxNat portId);

_AVXINL afxDrawContext AfxGetRasterContext(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxDrawContext dctx = AfxGetProvider(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVXINL void* AfxGetRasterUdd(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->udd;
}

_AVXINL afxNat AfxCountRasterSamples(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->sampleCnt;
}

_AVXINL avxFormat AfxGetRasterFormat(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->fmt;
}

_AVXINL afxRasterUsage AfxGetRasterUsage(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->usage;
}

_AVXINL afxRasterUsage AfxTestRasterUsage(afxRaster ras, afxRasterUsage mask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->usage & mask;
}

_AVXINL afxRasterFlags AfxGetRasterFlags(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->flags;
}

_AVXINL afxRasterFlags AfxTestRasterFlags(afxRaster ras, afxRasterFlags mask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->flags & mask;
}

_AVXINL afxNat AfxGetRasterBpp(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return AfxGetBpp(ras->fmt);
}

_AVXINL afxNat AfxCountRasterMipmaps(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->lodCnt;
}

_AVXINL afxBool AfxRasterIs1d(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return AfxTestRasterFlags(ras, afxRasterFlag_1D);
}

_AVXINL afxBool AfxRasterIs3d(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return AfxTestRasterFlags(ras, afxRasterFlag_3D);
}

_AVXINL afxBool AfxRasterIsCubemap(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return AfxTestRasterFlags(ras, afxRasterFlag_CUBEMAP);
}

_AVXINL afxBool AfxRasterIsLayered(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return AfxTestRasterFlags(ras, afxRasterFlag_LAYERED);
}

_AVXINL void AfxGetRasterSwizzling(afxRaster ras, afxNat subIdx, avxColorSwizzling* csw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->subCnt, subIdx, 1);
    AfxAssert(csw);
    *csw = ras->subs[subIdx].swizzling;
}

_AVXINL void AfxGetRasterExtent(afxRaster ras, afxNat lodIdx, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    
    AfxAssert(whd);
    whd[0] = ras->whd[0];
    whd[1] = ras->whd[1];
    whd[2] = ras->whd[2];

    while (lodIdx--)
    {
        whd[0] = whd[0] >> 1;
        whd[1] = whd[1] >> 1;
        whd[2] = whd[2] >> 1;
    }
    AfxWhdMax(whd, AFX_WHD_IDENTITY, whd);
}

_AVXINL void AfxDescribeRaster(afxRaster ras, afxRasterInfo* desc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(desc);
    desc->lodCnt = ras->lodCnt;
    AfxWhdCopy(desc->whd, ras->whd);
    desc->fmt = ras->fmt;
    desc->flags = ras->flags;
    desc->usage = ras->usage;
}

_AVX afxBool AfxQueryRasterLayout(afxRaster ras, afxNat lodIdx, afxNat layerIdx, afxRasterLayout* layout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssertRange(ras->whd[2], layerIdx, 1);

    afxBool is3d = AfxTestRasterFlags(ras, afxRasterFlag_3D);
    AfxAssert(!layerIdx || !is3d);

    afxWhd whd;
    AfxWhdMax(whd, AFX_WHD_IDENTITY, ras->whd);

    while (lodIdx--)
    {
        whd[0] = whd[0] >> 1;
        whd[1] = whd[1] >> 1;

        if (is3d)
            whd[2] = whd[2] >> 1;
    }
    AfxWhdMax(whd, AFX_WHD_IDENTITY, whd); // clamp

    afxNat pixelStride = AFX_ALIGNED_SIZEOF(AfxGetRasterBpp(ras), AFX_BYTE_SIZE) / AFX_BYTE_SIZE;
    afxNat rowStride = AFX_ALIGNED_SIZEOF(pixelStride * whd[0], AFX_SIMD_ALIGN);
    afxNat depthStride = rowStride * whd[1];
    AfxAssert(layout);
    layout->depthStride = depthStride;
    layout->rowStride = rowStride;
    layout->offset = 0;
    layout->siz = is3d ? whd[2] * depthStride : depthStride;
    return !err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUpdateRaster(afxRaster ras, afxNat opCnt, afxRasterIo const ops[], void const* src, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(src);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        avxTransference transfer = { 0 };

        transfer.dstFcc = afxFcc_RAS;
        transfer.img.ras = ras;
        transfer.img.src = src;

#if _AFX_DEBUG
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(ras->whd, op->rgn.origin, op->rgn.whd);
        }
#endif

        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxDumpRaster(afxRaster ras, afxNat opCnt, afxRasterIo const ops[], void* dst, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(dst);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        avxTransference transfer = { 0 };

        transfer.srcFcc = afxFcc_RAS;
        transfer.img.ras = ras;
        transfer.img.dst = dst;

#if _AFX_DEBUG
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(ras->whd, op->rgn.origin, op->rgn.whd);
        }
#endif

        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxPackRaster(afxRaster ras, afxNat opCnt, afxRasterIo const ops[], afxBuffer buf, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        avxTransference transfer = { 0 };

#if _AFX_DEBUG
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(ras->whd, op->rgn.origin, op->rgn.whd);
        }
#endif

        transfer.dstFcc = afxFcc_RAS;
        transfer.srcFcc = afxFcc_BUF;
        transfer.img.ras = ras;
        transfer.img.buf = buf;

        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxUnpackRaster(afxRaster ras, afxNat opCnt, afxRasterIo const ops[], afxBuffer buf, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        avxTransference transfer = { 0 };

        transfer.srcFcc = afxFcc_RAS;
        transfer.dstFcc = afxFcc_BUF;
        transfer.img.ras = ras;
        transfer.img.buf = buf;

#if _AFX_DEBUG
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(ras->whd, op->rgn.origin, op->rgn.whd);
        }
#endif

        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUploadRaster(afxRaster ras, afxNat opCnt, afxRasterIo const ops[], afxStream in, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(opCnt);
    AfxAssert(ops);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        avxTransference transfer = { 0 };

#if _AFX_DEBUG
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(ras->whd, op->rgn.origin, op->rgn.whd);
        }
#endif

        transfer.dstFcc = afxFcc_RAS;
        transfer.srcFcc = afxFcc_IOB;
        transfer.img.ras = ras;
        transfer.img.iob = in;

        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxDownloadRaster(afxRaster ras, afxNat opCnt, afxRasterIo const ops[], afxStream out, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(opCnt);
    AfxAssert(ops);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        avxTransference transfer = { 0 };

        transfer.srcFcc = afxFcc_RAS;
        transfer.dstFcc = afxFcc_IOB;
        transfer.img.ras = ras;
        transfer.img.iob = out;

#if _AFX_DEBUG
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(ras->whd, op->rgn.origin, op->rgn.whd);
        }
#endif

        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxPrintRaster(afxRaster ras, afxRasterIo const* iop, afxNat lodCnt, afxUri const* uri, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_WRITEABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReopenFile(file, uri, afxFileFlag_W)) AfxThrowError();
    else
    {
        afxNat maxLodCnt = AfxCountRasterMipmaps(ras);
        AfxAssertRange(maxLodCnt, 0, lodCnt);
        afxNat lodCnt = AfxMin(AfxMax(1, lodCnt), maxLodCnt);

        afxRasterIo iopClamped = { 0 };
        AfxGetRasterExtent(ras, 0, iopClamped.rgn.whd);

        if (iop)
        {
            AfxAssertRange(maxLodCnt, iop->rgn.lodIdx, lodCnt);
            afxNat lodIdxClamped = AfxMin(iop->rgn.lodIdx, maxLodCnt - 1);
            lodCnt = AfxMin(lodCnt, maxLodCnt - lodIdxClamped);

            afxWhd maxWhd, maxXyz;
            AfxGetRasterExtent(ras, lodIdxClamped, maxWhd);
            AfxAssertRangeWhd(maxWhd, iop->rgn.origin, iop->rgn.whd);

            AfxWhdClampOffset(iopClamped.rgn.origin, iop->rgn.origin, maxWhd);

            afxWhd maxRgnWhd;
            AfxWhdSub(maxRgnWhd, maxWhd, iopClamped.rgn.origin);
            AfxWhdClamp(iopClamped.rgn.whd, iop->rgn.whd, AFX_WHD_IDENTITY, maxRgnWhd);

            iopClamped.rgn = iop->rgn;
            iopClamped.rgn.lodIdx = lodIdxClamped;
        }

        afxRasterLayout layout;
        AfxQueryRasterLayout(ras, 0, 0, &layout);
        avxFormat fmt = AfxGetRasterFormat(ras);

        if (_AfxPrintRasterToTarga(ras, &iopClamped, lodCnt, (afxStream)file, portId))
            AfxThrowError();

        afxDrawContext dctx = AfxGetRasterContext(ras);
        AfxWaitForDrawBridge(dctx, portId, AFX_TIME_INFINITE); // we need to wait for completation before releasing the stream.

        AfxReleaseObjects(1, &file);
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxRasStdDtorCb(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    return err;
}

_AVX afxError _AvxRasStdCtorCb(afxRaster ras, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxRasterInfo const *texi = ((afxRasterInfo const *)args[1]) + invokeNo;

    afxDrawContext dctx = AfxGetProvider(ras);

    ras->subCnt = 0;
    ras->subs = NIL;

    //ras->swizzling = NIL; //&AFX_STD_COLOR_SWIZZLING;
    
    ras->lodCnt = AfxMax(texi->lodCnt, 1);
    AfxWhdMax(ras->whd, AFX_WHD_IDENTITY, texi->whd);

    ras->flags = NIL;;
    ras->usage = texi->usage;

    ras->fmt = texi->fmt;

    afxRasterFlags flags = (afxRasterFlag_SUBSAMPLED | afxRasterFlag_RESAMPLED);

    if ((ras->flags = (texi->flags & flags)) == flags) AfxThrowError();
    else
    {
        if (ras->lodCnt > 1)
            if (!(ras->flags & flags))
                ras->flags |= afxRasterFlag_SUBSAMPLED;

        flags = (afxRasterFlag_3D | /*afxRasterFlag_CUBEMAP | */afxRasterFlag_LAYERED);

        if ((ras->flags = (texi->flags & flags)) == flags) AfxThrowError();
        else
        {
            if (ras->whd[2] > 1)
                if (!(ras->flags & flags))
                    ras->flags = afxRasterFlag_LAYERED;
        }
    }

    ras->sub0.baseLod = 0;
    ras->sub0.lodCnt = 1;
    ras->sub0.baseLayer = 0;
    ras->sub0.layerCnt = 1;
    ras->sub0.flags = ras->flags;
    ras->sub0.fmt = ras->fmt;
    ras->sub0.swizzling.r = avxColorSwizzle_R;
    ras->sub0.swizzling.g = avxColorSwizzle_B;
    ras->sub0.swizzling.b = avxColorSwizzle_G;
    ras->sub0.swizzling.a = avxColorSwizzle_A;

    ras->udd = texi->udd;

    return err;
}

_AVX afxClassConfig const _AvxRasStdImplementation =
{
    .fcc = afxFcc_RAS,
    .name = "Raster",
    .desc = "Formatted Video Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxRaster)),
    .ctor = (void*)_AvxRasStdCtorCb,
    .dtor = (void*)_AvxRasStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireRasters(afxDrawContext dctx, afxNat cnt, afxRasterInfo const info[], afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(rasters);
    AfxAssert(info);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxRasterFlags flags = info[i].flags;

        if ((flags & afxRasterFlag_CUBEMAP) && (info[i].whd[2] % 6))
        {
            AfxAssert(info[i].whd[2] % 6 == 0);
            AfxThrowError();

            if ((info[i].whd[2] / 6) && !(flags & afxRasterFlag_LAYERED))
            {
                AfxAssertAND((info[i].whd[2] / 6), (flags & afxRasterFlag_LAYERED));
                AfxThrowError();
            }
        }

        if ((flags & afxRasterFlag_1D) && (info[i].whd[1] > 1))
        {
            AfxAssert(1 >= info[i].whd[1]);
            AfxThrowError();

            if ((info[i].whd[2] > 1) && !(flags & afxRasterFlag_LAYERED))
            {
                AfxAssertOR(1 >= info[i].whd[2], flags & afxRasterFlag_LAYERED);
                AfxThrowError();
            }
        }

        if ((flags & afxRasterFlag_2D) && (info[i].whd[2] > 1) && !(flags & afxRasterFlag_LAYERED))
        {
            AfxThrowError();

            if ((info[i].whd[2] > 1) && !(flags & afxRasterFlag_LAYERED))
            {
                AfxAssertOR(1 >= info[i].whd[2], flags & afxRasterFlag_LAYERED);
                AfxThrowError();
            }
        }
    }

    afxClass* cls = AfxGetRasterClass(dctx);
    AfxAssertClass(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dctx, info }))
        AfxThrowError();

    AfxAssertObjects(cnt, rasters, afxFcc_RAS);

    return err;
}

_AVX afxRaster AfxAssembleRaster(afxDrawContext dctx, afxRasterInfo const* info, afxUri const* dir, afxNat cnt, afxUri const layers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(layers);
    AfxAssert(cnt);
    afxRaster ras;

    if (AfxAssembleRasters(dctx, info, dir, cnt, layers, &ras))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}

_AVX afxRaster AfxAssembleCubemapRasters(afxDrawContext dctx, afxRasterInfo const* info, afxUri const* dir, afxUri const faces[6])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(faces);
    afxRaster ras;

    afxRasterInfo info2 = *info;
    info2.flags |= afxRasterFlag_CUBEMAP;
    info2.usage |= afxRasterUsage_SAMPLING;
    info2.whd[2] = 6;

    if (!(ras = AfxAssembleRaster(dctx, &info2, dir, 6, faces)))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}
