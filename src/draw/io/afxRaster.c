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

AVX afxError _AfxPrintRasterToTarga(afxRaster ras, afxRasterIo const* iop, afxUnit lodCnt, afxStream out, afxUnit portId);
AVX afxError _AfxFetchRasterFromTarga(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit lodCnt, afxStream in, afxUnit portId);

_AVXINL afxDrawContext AfxGetRasterContext(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxDrawContext dctx = AfxGetProvider(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVXINL afxDrawDevice AfxGetRasterDevice(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxDrawContext dctx = AfxGetProvider(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVXINL avxFormat AfxGetRasterFormat(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->fmt;
}

_AVXINL afxRasterUsage AfxTestRasterUsage(afxRaster ras, afxRasterUsage mask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->usage & mask;
}

_AVXINL afxRasterFlags AfxTestRasterFlags(afxRaster ras, afxRasterFlags mask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->flags & mask;
}

_AVXINL void AfxGetRasterSwizzling(afxRaster ras, afxUnit subIdx, avxColorSwizzling* csw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AFX_ASSERT_RANGE(ras->subCnt, subIdx, 1);
    AFX_ASSERT(csw);
    *csw = ras->subs[subIdx].swizzling;
}

_AVXINL afxWhd AfxGetRasterExtent(afxRaster ras, afxUnit lodIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AFX_ASSERT_RANGE(ras->lodCnt, lodIdx, 1);        
    afxWhd whd = ras->extent;
    
    while (lodIdx--)
    {
        whd.w = whd.w >> 1;
        whd.h = whd.h >> 1;
        whd.d = whd.d >> 1;
    }
    AfxWhdMax(&whd, AfxWhd(1, 1, 1), whd);
    return whd;
}

_AVXINL void AfxDescribeRaster(afxRaster ras, afxRasterInfo* desc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AFX_ASSERT(desc);
    desc->lodCnt = ras->lodCnt;
    desc->extent = AfxGetRasterExtent(ras, 0);
    desc->fmt = ras->fmt;
    desc->flags = ras->flags;
    desc->usage = ras->usage;
}

_AVX afxBool AfxQueryRasterLayout(afxRaster ras, afxUnit lodIdx, afxUnit layerIdx, afxRasterLayout* layout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AFX_ASSERT_RANGE(ras->lodCnt, lodIdx, 1);
    AFX_ASSERT_RANGE(ras->extent.d, layerIdx, 1);

    afxBool is3d = AfxTestRasterFlags(ras, afxRasterFlag_3D);
    AFX_ASSERT(!layerIdx || !is3d);

    afxWhd whd = AfxGetRasterExtent(ras, lodIdx);
    
    afxUnit pixelStride = AFX_ALIGNED_SIZE(AfxGetBpp(ras->fmt), AFX_BYTE_SIZE) / AFX_BYTE_SIZE;
    afxUnit rowStride = AFX_ALIGNED_SIZE(pixelStride * whd.w, AFX_SIMD_ALIGNMENT);
    afxUnit depthStride = whd.h * rowStride;
    AFX_ASSERT(layout);
    layout->depthStride = depthStride;
    layout->rowStride = rowStride;
    layout->offset = 0;
    layout->siz = is3d ? whd.d * depthStride : depthStride;
    return !err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUpdateRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], void const* src, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(src);

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
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(AfxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
        }
#endif

        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxDumpRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], void* dst, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

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
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(AfxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
        }
#endif

        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxPackRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxBuffer buf, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        avxTransference transfer = { 0 };

#if _AFX_DEBUG
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(AfxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
        }
#endif

        transfer.dstFcc = afxFcc_RAS;
        transfer.srcFcc = afxFcc_BUF;
        transfer.img.ras = ras;
        transfer.img.buf = buf;

        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxUnpackRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxBuffer buf, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

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
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(AfxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
        }
#endif

        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUploadRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxStream in, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        avxTransference transfer = { 0 };

#if _AFX_DEBUG
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(AfxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
        }
#endif

        transfer.dstFcc = afxFcc_RAS;
        transfer.srcFcc = afxFcc_IOB;
        transfer.img.ras = ras;
        transfer.img.iob = in;

        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxDownloadRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxStream out, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

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
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxRasterIo const* op = &ops[i];
            AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
            AfxAssertRangeWhd(AfxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
        }
#endif

        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxPrintRaster(afxRaster ras, afxRasterIo const* iop, afxUnit lodCnt, afxUri const* uri, afxUnit portId)
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
        afxUnit maxLodCnt = ras->lodCnt;
        AFX_ASSERT_RANGE(maxLodCnt, 0, lodCnt);
        lodCnt = lodCnt ? AfxMin(lodCnt, maxLodCnt) : maxLodCnt;

        afxRasterIo iopClamped = { 0 };
        iopClamped.rgn.extent = AfxGetRasterExtent(ras, 0);

        if (iop)
        {
            AFX_ASSERT_RANGE(maxLodCnt, iop->rgn.lodIdx, lodCnt);
            afxUnit lodIdxClamped = AfxMin(iop->rgn.lodIdx, maxLodCnt - 1);
            lodCnt = AfxMin(lodCnt, maxLodCnt - lodIdxClamped);

            afxWhd maxWhd, maxXyz;
            maxWhd = AfxGetRasterExtent(ras, lodIdxClamped);
            AfxAssertRangeWhd(maxWhd, iop->rgn.origin, iop->rgn.extent);

            AfxWhdClampOffset(&iopClamped.rgn.origin, iop->rgn.origin, maxWhd);

            afxWhd maxRgnWhd;
            AfxWhdSub(&maxRgnWhd, maxWhd, iopClamped.rgn.origin);
            AfxWhdClamp(&iopClamped.rgn.extent, iop->rgn.extent, AfxWhd(1, 1, 1), maxRgnWhd);

            iopClamped.rgn = iop->rgn;
            iopClamped.rgn.lodIdx = lodIdxClamped;
        }

        afxRasterLayout layout;
        AfxQueryRasterLayout(ras, 0, 0, &layout);
        avxFormat fmt = AfxGetRasterFormat(ras);

        avxFormatDescription pfd;
        AfxDescribePixelFormat(fmt, &pfd);

        afxTargaFlags flags = NIL;

        if (AfxTestRasterFlags(ras, afxRasterFlag_CUBEMAP))
            flags |= afxTargaFlag_CUBEMAP;

        if (AfxTestRasterFlags(ras, afxRasterFlag_3D))
            flags |= afxTargaFlag_3D;

        iopClamped.rowStride = AFX_ALIGNED_SIZE(iopClamped.rgn.extent.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        iopClamped.rowsPerImg = iopClamped.rgn.extent.h;

        if (AfxWriteTarga(file, &iopClamped, lodCnt, fmt, flags, 0, NIL)) AfxThrowError();
        else
        {
            // write data for each segment

            afxRasterIo iopTmp = iopClamped;

            for (afxUnit i = 0; i < lodCnt; i++)
            {
                afxRasterLayout lodLayout;
                AfxQueryRasterLayout(ras, i, 0, &lodLayout);

                iopTmp.rgn.lodIdx = i;
                iopTmp.offset = AfxAskStreamPosn(file);
                iopTmp.rowStride = iopClamped.rowStride;// lodLayout.rowStride;
                iopTmp.rowsPerImg = iopClamped.rowsPerImg;

                if (AfxDownloadRaster(ras, 1, &iopTmp, file, portId))
                    AfxThrowError();

                AfxWhdHalf(&iopTmp.rgn.origin, iopTmp.rgn.origin);
                AfxWhdHalf(&iopTmp.rgn.extent, iopTmp.rgn.extent);
            }
        }

        afxDrawContext dctx = AfxGetRasterContext(ras);
        AfxWaitForDrawBridge(dctx, portId, AFX_TIME_INFINITE); // we need to wait for completation before releasing the stream.
    }
    AfxReleaseObjects(1, &file);
    return err;
}

_AVX afxError AfxFetchRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit lodCnt, afxUri const* uri, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AFX_ASSERT(uri);

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReloadFile(file, uri))
    {
        AfxThrowError();
        return err;
    }
    
    afxTargaFile tgai = { 0 };
    if (AfxReadTarga(file, &tgai)) AfxThrowError();
    else
    {
        afxDrawContext dctx = AfxGetRasterContext(ras);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxRasterLayout layout;
        AfxQueryRasterLayout(ras, 0, 0, &layout);
        avxFormat fmt = AfxGetRasterFormat(ras);

        afxUnit maxLodCnt = ras->lodCnt;
        AFX_ASSERT_RANGE(maxLodCnt, 0, lodCnt);
        lodCnt = AfxMin(AfxMax(1, lodCnt), maxLodCnt);

        afxRasterIo iopClamped = { 0 };
        iopClamped.rgn.extent = AfxGetRasterExtent(ras, 0);

        if (opCnt)
        {
            AFX_ASSERT(ops);

            for (afxUnit i = 0; i < opCnt; i++)
            {
                afxRasterIo const* iop = &ops[i];

                AFX_ASSERT_RANGE(maxLodCnt, iop->rgn.lodIdx, lodCnt);
                afxUnit lodIdxClamped = AfxMin(iop->rgn.lodIdx, maxLodCnt - 1);
                lodCnt = AfxMin(lodCnt, maxLodCnt - lodIdxClamped);

                afxWhd maxWhd, maxXyz;
                maxWhd = AfxGetRasterExtent(ras, lodIdxClamped);
                AfxAssertRangeWhd(maxWhd, iop->rgn.origin, iop->rgn.extent);

                AfxWhdClampOffset(&iopClamped.rgn.origin, iop->rgn.origin, maxWhd);

                afxWhd maxRgnWhd;
                AfxWhdSub(&maxRgnWhd, maxWhd, iopClamped.rgn.origin);
                AfxWhdClamp(&iopClamped.rgn.extent, iop->rgn.extent, AfxWhd(1, 1, 1), maxRgnWhd);

                iopClamped.rgn = iop->rgn;
                iopClamped.rgn.lodIdx = lodIdxClamped;

                void* data = AfxAllocate(tgai.decSiz, 1, AFX_SIMD_ALIGNMENT, AfxHere());

                AfxDecodeTarga(file, &tgai, data);
                AfxUpdateRaster(ras, 1, &iopClamped, data, portId);

                AfxWaitForDrawBridge(AfxGetRasterContext(ras), portId, AFX_TIME_INFINITE);

                AfxDeallocate(data);
            }
        }
        else
        {
            afxRasterIo op = { 0 };
            op.rgn.extent.w = tgai.width;
            op.rgn.extent.h = tgai.height;
            op.rgn.extent.d = tgai.depth;
            op.offset = 0;
            op.rowStride = tgai.rowStride;
            op.rowsPerImg = tgai.height;

            void* data = AfxAllocate(tgai.decSiz, 1, AFX_SIMD_ALIGNMENT, AfxHere());

            AfxDecodeTarga(file, &tgai, data);
            AfxUpdateRaster(ras, 1, &op, data, portId);

            AfxWaitForDrawBridge(AfxGetRasterContext(ras), portId, AFX_TIME_INFINITE);

            AfxDeallocate(data);
        }

        //AfxWaitForDrawBridge(dctx, portId, AFX_TIME_INFINITE); // we need to wait for completation before releasing the stream.
    }
    AfxReleaseObjects(1, &file);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxRasStdDtorCb(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    return err;
}

_AVX afxError _AvxRasStdCtorCb(afxRaster ras, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxRasterInfo const* rasi = ((afxRasterInfo const *)args[1]) + invokeNo;

    afxDrawContext dctx = AfxGetProvider(ras);

    ras->subCnt = 0;
    ras->subs = NIL;

    //ras->swizzling = NIL; //&AFX_STD_COLOR_SWIZZLING;
    
    ras->lodCnt = AfxMax(rasi->lodCnt, 1);
    AfxWhdMax(&ras->extent, AfxWhd(1, 1, 1), rasi->extent);

    ras->flags = NIL;;
    ras->usage = rasi->usage;

    ras->fmt = rasi->fmt;

    afxRasterFlags flags = (afxRasterFlag_MIPMAP | afxRasterFlag_RESAMPLED);

    if ((ras->flags = (rasi->flags & flags)) == flags) AfxThrowError();
    else
    {
        if (ras->lodCnt > 1)
            if (!(ras->flags & flags))
                ras->flags |= afxRasterFlag_MIPMAP;

        flags = (afxRasterFlag_3D | /*afxRasterFlag_CUBEMAP | */afxRasterFlag_MULTIMAP);

        if ((ras->flags = (rasi->flags & flags)) == flags) AfxThrowError();
        else
        {
            if (ras->extent.d > 1)
                if (!(ras->flags & flags))
                    ras->flags = afxRasterFlag_MULTIMAP;
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

    ras->udd = rasi->udd;

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

_AVX afxError AfxAcquireRasters(afxDrawContext dctx, afxUnit cnt, afxRasterInfo const info[], afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(rasters);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxRasterFlags flags = info[i].flags;

        if ((flags & afxRasterFlag_CUBEMAP) && (info[i].extent.d % 6))
        {
            AFX_ASSERT(info[i].extent.d % 6 == 0);
            AfxThrowError();

            if ((info[i].extent.d / 6) && !(flags & afxRasterFlag_MULTIMAP))
            {
                AfxAssertAND((info[i].extent.d / 6), (flags & afxRasterFlag_MULTIMAP));
                AfxThrowError();
            }
        }

        if ((flags & afxRasterFlag_1D) && (info[i].extent.h > 1))
        {
            AFX_ASSERT(1 >= info[i].extent.h);
            AfxThrowError();

            if ((info[i].extent.d > 1) && !(flags & afxRasterFlag_MULTIMAP))
            {
                AfxAssertOR(1 >= info[i].extent.d, flags & afxRasterFlag_MULTIMAP);
                AfxThrowError();
            }
        }

        if ((flags & afxRasterFlag_2D) && (info[i].extent.d > 1) && !(flags & afxRasterFlag_MULTIMAP))
        {
            AfxThrowError();

            if ((info[i].extent.d > 1) && !(flags & afxRasterFlag_MULTIMAP))
            {
                AfxAssertOR(1 >= info[i].extent.d, flags & afxRasterFlag_MULTIMAP);
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

_AVX afxError AfxLoadRasters(afxDrawContext dctx, afxUnit cnt, afxRasterInfo const info[], afxUri const uri[], afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(rasters);
    //AFX_ASSERT(info);
    AFX_ASSERT(uri);
    AFX_ASSERT(cnt);

    afxUnit portId = 0;
    afxStream file = NIL;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    afxArena arena;
    AfxDeployArena(&arena, NIL, AfxHere());

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (AfxReopenFile(file, &uri[i], afxFileFlag_R)) AfxThrowError();
        else
        {
            afxTargaFile tgai = { 0 };

            if (AfxReadTarga(file, &tgai))
            {
                AfxThrowError();
                rasters[i] = NIL;
            }
            else
            {
                avxFormat fmt = tgai.fmt;

                afxRasterInfo rasi = { 0 };

                if (info)
                {
                    rasi.fmt = info[i].fmt ? info[i].fmt : fmt;
                    rasi.lodCnt = info[i].lodCnt;
                    rasi.usage = info[i].usage;
                    rasi.flags = info[i].flags;

                    rasi.extent.w = AfxMax(1, AfxMax(info[i].extent.w, tgai.width));
                    rasi.extent.h = AfxMax(1, AfxMax(info[i].extent.h, tgai.height));
                    rasi.extent.d = AfxMax(1, AfxMax(info[i].extent.d, tgai.depth));

                    rasi.flags = info[i].flags | (tgai.flags & afxTargaFlag_CUBEMAP ? afxRasterFlag_CUBEMAP : NIL);
                    rasi.usage = info[i].usage ? info[i].usage : afxRasterUsage_SAMPLING | afxRasterUsage_SRC;
                }
                else
                {
                    rasi.fmt = tgai.fmt;
                    rasi.lodCnt = tgai.lodCnt;
                    rasi.flags = tgai.flags & afxTargaFlag_CUBEMAP ? afxRasterFlag_CUBEMAP : NIL;
                    rasi.usage = afxRasterUsage_SAMPLING | afxRasterUsage_SRC;

                    rasi.extent.w = tgai.width;
                    rasi.extent.h = tgai.height;
                    rasi.extent.d = tgai.depth;
                }

                if (AfxAcquireRasters(dctx, 1, &rasi, &rasters[i]))
                {
                    AfxThrowError();
                    AfxReleaseObjects(i, (void**)rasters);
                }
                else
                {
                    afxRasterIo op = { 0 };
                    op.rgn.extent.w = tgai.width;
                    op.rgn.extent.h = tgai.height;
                    op.rgn.extent.d = tgai.depth;
                    op.offset = 0;
                    op.rowStride = tgai.rowStride;
                    //op.rowCnt = tgai.extent[1];
                    //op.decSiz = tgai.decSiz;

                    void* data = AfxAllocateArena(&arena, tgai.decSiz);

                    AfxDecodeTarga(file, &tgai, data);
                    AfxUpdateRaster(rasters[i], 1, &op, data, portId);
                }
            }
        }
    }

    AfxWaitForDrawBridge(dctx, portId, AFX_TIME_INFINITE);

    AfxDismantleArena(&arena);

    if (file)
        AfxReleaseObjects(1, &file);

    return err;
}

_AVX afxRaster AfxLoadCubemapRaster(afxDrawContext dctx, afxRasterInfo const* info, afxUri const* dir, afxUri const faces[6])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(faces);
    afxRaster ras;

    afxRasterInfo info2 = *info;
    info2.flags |= afxRasterFlag_CUBEMAP;
    info2.usage |= afxRasterUsage_SAMPLING;
    info2.extent.d = 6;

    afxUri2048 urib;
    AfxMakeUri2048(&urib, NIL);
    AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(AfxGetUriString(dir)), AfxPushString(AfxGetUriString(&faces[0])));

    if (AfxLoadRasters(dctx, 1, &info2, &urib.uri, &ras)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ras, afxFcc_RAS);

        afxRasterIo op = { 0 };
        op.rgn.extent = AfxGetRasterExtent(ras, info2.lodCnt);
        op.rgn.extent.d = 1;

        for (afxUnit i = 1; i < 6; i++) // start at 1; base is already loaded.
        {
            op.rgn.origin.z = i;
            
            AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(AfxGetUriString(dir)), AfxPushString(AfxGetUriString(&faces[i])));

            if (AfxFetchRaster(ras, 1, &op, info2.lodCnt, &urib.uri, 0))
                AfxThrowError();
        }
    }
    return ras;
}
