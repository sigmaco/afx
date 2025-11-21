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

#define _AVX_DRAW_C
#define _AVX_RASTER_C
#include "avxIcd.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x whd >> lod

AVX afxError _AfxPrintRasterToTarga(avxRaster ras, avxRasterIo const* iop, afxUnit lodCnt, afxStream out, afxUnit portId);
AVX afxError _AfxFetchRasterFromTarga(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit lodCnt, afxStream in, afxUnit portId);

_AVXINL void _AvxSanitizeRasterIo(avxRaster ras, afxSize bufCap, afxUnit cnt, avxRasterIo const raw[], avxRasterIo san[])
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);
    AFX_ASSERT(cnt);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterIo struct to be within the bounds of a raster object.
    */

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterIo* s = &san[i];
        avxRasterIo const* r = &raw[i];
        _AvxSanitizeRasterRegion(ras, 1, &r->rgn, &s->rgn);
        s->offset = AFX_MIN(r->offset, bufCap - 1);
        s->rowStride = AFX_MIN(r->rowStride, bufCap - s->offset);
        s->rowsPerImg = AFX_MIN(r->rowsPerImg, (s->rowStride ? bufCap / s->rowStride : 0));

        // rowStride MUST be 0, or greater than or equal to the whd.w.
        AFX_ASSERT(s->rowStride >= s->rgn.whd.w);
        // rowCnt MUST be 0, or greater than or equal to the whd.h.
        AFX_ASSERT(s->rowsPerImg >= s->rgn.whd.h);
        // whd.w MUST not be 0.
        // whd.h MUST not be 0.
        // whd.d MUST not be 0.
        AFX_ASSERT(s->rgn.whd.w);
        AFX_ASSERT(s->rgn.whd.h);
        AFX_ASSERT(s->rgn.whd.d);
    }
}

_AVX afxError AvxUpdateRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], void const* src, afxFlags flags, afxMask exuMask)
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(src);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(ras->mipCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.whd);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_RAS;
    transfer.dst.ras = ras;
    transfer.src.src = src;

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxDsysGetDdi(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

_AVX afxError AvxDumpRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], void* dst, afxFlags flags, afxMask exuMask)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

#if AVX_VALIDATION_ENABLED

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(ras->mipCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.whd);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_RAS;
    transfer.src.ras = ras;
    transfer.dst.dst = dst;

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    
    if (_AvxDsysGetDdi(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxCopyRaster(avxRaster ras, afxUnit opCnt, avxRasterCopy const ops[], avxRaster src, afxFlags flags, afxMask exuMask)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    // @src must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    avxRasterInfo rasi, rasi2;
    AvxDescribeRaster(src, &rasi, NIL, NIL);
    AvxDescribeRaster(ras, &rasi2, NIL, NIL);

#if AVX_VALIDATION_ENABLED

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterCopy const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.whd, op->src.origin, op->src.whd);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dstLodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.whd, op->dstOrigin, op->src.whd);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_RAS;
    transfer.dstFcc = afxFcc_RAS;
    transfer.src.ras = src;
    transfer.dst.ras = ras;

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxDsysGetDdi(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

_AVX afxError AvxPackRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], avxBuffer buf, afxMask exuMask)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

#if AVX_VALIDATION_ENABLED

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(ras->mipCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.whd);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_RAS;
    transfer.dstFcc = afxFcc_BUF;
    transfer.src.ras = ras;
    transfer.dst.buf = buf;

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxDsysGetDdi(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

_AVX afxError AvxUnpackRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], avxBuffer buf, afxMask exuMask)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(ras->mipCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.whd);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_RAS;
    transfer.srcFcc = afxFcc_BUF;
    transfer.dst.ras = ras;
    transfer.src.buf = buf;

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxDsysGetDdi(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxUploadRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxStream in, afxFlags flags, afxMask exuMask)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    // @in must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

#if AVX_VALIDATION_ENABLED

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(ras->mipCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.whd);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_RAS;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.ras = ras;
    transfer.src.iob = in;

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxDsysGetDdi(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

_AVX afxError AvxDownloadRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxStream out, afxFlags flags, afxMask exuMask)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    // @in must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(ras->mipCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.whd);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_RAS;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.ras = ras;
    transfer.dst.iob = out;

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxDsysGetDdi(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxPrintRaster(avxRaster ras, avxRasterIo const* iop, afxUnit lodCnt, afxUri const* uri, afxMask exuMask)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_WRITEABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReopenFile(file, uri, afxFileFlag_W))
    {
        AfxThrowError();
        return err;
    }

    afxUnit maxLodCnt = ras->mipCnt;
    AFX_ASSERT_RANGE(maxLodCnt, 0, lodCnt);
    lodCnt = lodCnt ? AFX_MIN(lodCnt, maxLodCnt) : maxLodCnt;

    avxRasterIo iopClamped = { 0 };
    iopClamped.rgn.whd = AvxGetRasterExtent(ras, 0);

    if (iop)
    {
        AFX_ASSERT_RANGE(maxLodCnt, iop->rgn.lodIdx, lodCnt);
        afxUnit lodIdxClamped = AFX_MIN(iop->rgn.lodIdx, maxLodCnt - 1);
        lodCnt = AFX_MIN(lodCnt, maxLodCnt - lodIdxClamped);

        avxRange maxWhd, maxXyz;
        maxWhd = AvxGetRasterExtent(ras, lodIdxClamped);
        AFX_ASSERT_RANGE_WHD(maxWhd, iop->rgn.origin, iop->rgn.whd);

        iopClamped.rgn.origin = AvxClampOrigin(iop->rgn.origin, maxWhd);

        avxRange maxRgnWhd;
        maxRgnWhd = AvxSubtractRange(maxWhd, AVX_RANGE(iopClamped.rgn.origin.x, iopClamped.rgn.origin.y, iopClamped.rgn.origin.z));
        iopClamped.rgn.whd = AvxClampRange(iop->rgn.whd, AVX_RANGE(1, 1, 1), maxRgnWhd);

        iopClamped.rgn = iop->rgn;
        iopClamped.rgn.lodIdx = lodIdxClamped;
    }

    avxTgaFlags flags = NIL;

    if (AvxGetRasterFlags(ras, avxRasterFlag_CUBEMAP))
        flags |= avxTgaFlag_CUBEMAP;

    if (AvxGetRasterFlags(ras, avxRasterFlag_3D))
        flags |= avxTgaFlag_3D;

    iopClamped.rowStride = AFX_ALIGN_SIZE(iopClamped.rgn.whd.w * ras->fmtStride, AFX_SIMD_ALIGNMENT);
    iopClamped.rowsPerImg = iopClamped.rgn.whd.h;

    avxRasterFile tga;
    AvxPrepareRasterFile(&tga, &iopClamped, lodCnt, ras->fmt, (avxRasterFlags)flags, 0);

    if (AvxWriteRasterFile(&tga, NIL, 0, file)) AfxThrowError();
    else
    {
        // write data for each segment

        avxRasterIo iopTmp = iopClamped;

        for (afxUnit i = 0; i < lodCnt; i++)
        {
            iopTmp.rgn.lodIdx = i;
            iopTmp.offset = AfxAskStreamPosn(file);
            iopTmp.rowStride = iopClamped.rowStride;// lodLayout.rowStride;
            iopTmp.rowsPerImg = iopClamped.rowsPerImg;

            if (AvxDownloadRaster(ras, 1, &iopTmp, file, NIL, exuMask))
                AfxThrowError();

            iopTmp.rgn.origin = AvxHalfOrigin(iopTmp.rgn.origin);
            iopTmp.rgn.whd = AvxHalfRange(iopTmp.rgn.whd);
        }
    }

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, exuMask); // we need to wait for completation before releasing the stream.
    
    AfxDisposeObjects(1, &file);

    return err;
}

_AVX afxError AvxReloadRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit lodCnt, afxUri const* uri, afxMask exuMask)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(uri);

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReloadFile(file, uri))
    {
        AfxThrowError();
        return err;
    }
    
    avxRasterFile tgai = { 0 };
    if (AvxReadRasterFile(&tgai, file))
    {
        AfxThrowError();
        AfxDisposeObjects(1, &file);
        return err;
    }

    afxUnit maxLodCnt = ras->mipCnt;
    AFX_ASSERT_RANGE(maxLodCnt, 0, lodCnt);
    lodCnt = AFX_MIN(AFX_MAX(1, lodCnt), maxLodCnt);

    avxRasterIo iopClamped = { 0 };
    iopClamped.rgn.whd = AvxGetRasterExtent(ras, 0);

    if (opCnt == 0)
    {
        avxRasterIo op = { 0 };
        op.rgn.whd.w = tgai.width;
        op.rgn.whd.h = tgai.height;
        op.rgn.whd.d = tgai.depth;
        op.offset = 0;
        op.rowStride = tgai.rowStride;
        op.rowsPerImg = tgai.height;

        void* data;
        AfxAllocate(tgai.decSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&data);

        AvxDecodeRasterFile(&tgai, file, data);
        AvxUpdateRaster(ras, 1, &op, data, NIL, exuMask);

        AvxWaitForDrawBridges(AvxGetRasterHost(ras), AFX_TIMEOUT_INFINITE, exuMask);

        AfxDeallocate((void**)&data, AfxHere());
    }
    else
    {
        AFX_ASSERT(ops);

        for (afxUnit i = 0; i < opCnt; i++)
        {
            avxRasterIo const* iop = &ops[i];

            AFX_ASSERT_RANGE(maxLodCnt, iop->rgn.lodIdx, lodCnt);
            afxUnit lodIdxClamped = AFX_MIN(iop->rgn.lodIdx, maxLodCnt - 1);
            lodCnt = AFX_MIN(lodCnt, maxLodCnt - lodIdxClamped);

            avxRange maxWhd, maxXyz;
            maxWhd = AvxGetRasterExtent(ras, lodIdxClamped);
            AFX_ASSERT_RANGE_WHD(maxWhd, iop->rgn.origin, iop->rgn.whd);

            iopClamped.rgn.origin = AvxClampOrigin(iop->rgn.origin, maxWhd);

            avxRange maxRgnWhdClamped;
            maxRgnWhdClamped = AvxSubtractRange(maxWhd, AVX_RANGE(iopClamped.rgn.origin.x, iopClamped.rgn.origin.y, iopClamped.rgn.origin.z));
            iopClamped.rgn.whd = AvxClampRange(iop->rgn.whd, AVX_RANGE(AFX_OR(iop->rgn.whd.w, maxRgnWhdClamped.w), AFX_OR(iop->rgn.whd.h, maxRgnWhdClamped.h), AFX_OR(iop->rgn.whd.d, maxRgnWhdClamped.d)), maxRgnWhdClamped);

            //iopClamped.rgn = iop->rgn;
            iopClamped.rgn.lodIdx = lodIdxClamped;

            void* data;
            AfxAllocate(tgai.decSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&data);

            AvxDecodeRasterFile(&tgai, file, data);
            AvxUpdateRaster(ras, 1, &iopClamped, data, NIL, exuMask);

            AvxWaitForDrawBridges(AvxGetRasterHost(ras), AFX_TIMEOUT_INFINITE, exuMask);

            AfxDeallocate((void**)&data, AfxHere());
        }
    }
    //AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, portId); // we need to wait for completation before releasing the stream.
    
    AfxDisposeObjects(1, &file);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxLoadRasters(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const info[], afxUri const uri[], avxRasterFile tga[], avxRaster rasters[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(rasters);
    //AFX_ASSERT(info);
    AFX_ASSERT(uri);
    AFX_ASSERT(cnt);

    afxUnit portId = 0;
    afxStream file = NIL;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    afxMask exuMaskWait = NIL;

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (AfxReopenFile(file, &uri[i], afxFileFlag_R)) AfxThrowError();
        else
        {
            avxRasterFile tgai = { 0 };

            if (AvxReadRasterFile(&tgai, file))
            {
                AfxThrowError();
                rasters[i] = NIL;
                if (tga) tga[i] = tgai;
            }

            if (tga) tga[i] = tgai;

            avxFormat fmt = tgai.fmt;

            avxRasterInfo rasi = { 0 };

            if (info)
            {
                rasi.fmt = info[i].fmt ? info[i].fmt : fmt;
                rasi.lodCnt = info[i].lodCnt;
                rasi.usage = info[i].usage;
                rasi.flags = info[i].flags;

                rasi.exuMask = info[i].exuMask;

                rasi.whd.w = AFX_MAX(1, AFX_MAX(info[i].whd.w, tgai.width));
                rasi.whd.h = AFX_MAX(1, AFX_MAX(info[i].whd.h, tgai.height));
                rasi.whd.d = AFX_MAX(1, AFX_MAX(info[i].whd.d, tgai.depth));

                rasi.flags = info[i].flags | (tgai.flags & avxTgaFlag_CUBEMAP ? avxRasterFlag_CUBEMAP : NIL);
                rasi.usage = info[i].usage ? info[i].usage : avxRasterUsage_TEXTURE | avxRasterUsage_SRC;
            }
            else
            {
                rasi.fmt = tgai.fmt;
                rasi.lodCnt = tgai.lodCnt;
                rasi.flags = tgai.flags & avxTgaFlag_CUBEMAP ? avxRasterFlag_CUBEMAP : NIL;
                rasi.usage = avxRasterUsage_TEXTURE | avxRasterUsage_SRC;

                rasi.whd.w = tgai.width;
                rasi.whd.h = tgai.height;
                rasi.whd.d = tgai.depth;
            }

            if (AvxAcquireRasters(dsys, 1, &rasi, &rasters[i]))
            {
                AfxThrowError();
                AfxDisposeObjects(i, (void**)rasters);
            }
            else
            {
                avxRasterIo op = { 0 };
                op.rgn.whd.w = tgai.width;
                op.rgn.whd.h = tgai.height;
                op.rgn.whd.d = tgai.depth;
                op.offset = 0;
                op.rowStride = tgai.rowStride;
                op.rowsPerImg = tgai.rowsPerImg;
                //op.decSiz = tgai.decSiz;

                void* data = AfxRequestArena(&arena, tgai.decSiz, 1, NIL, 0);

                AvxDecodeRasterFile(&tgai, file, data);
                AvxUpdateRaster(rasters[i], 1, &op, data, NIL, rasi.exuMask);
                exuMaskWait |= rasi.exuMask;
            }
        }
    }

    AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, exuMaskWait);

    AfxDismantleArena(&arena);

    if (file)
        AfxDisposeObjects(1, &file);

    return err;
}

_AVX afxError AvxLoadSegmentedRaster(afxDrawSystem dsys, avxRasterInfo const* info, afxUri const* dir, afxUri const faces[], avxRaster* raster)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(faces);
    avxRaster ras;

    avxRasterInfo info2 = *info;
    //info2.flags |= avxRasterFlag_CUBEMAP;
    info2.usage |= avxRasterUsage_TEXTURE;
    //info2.whd.d = 6;

    afxUri2048 urib;
    AfxMakeUri2048(&urib, NIL);
    AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(AfxGetUriString(dir)), AfxPushString(AfxGetUriString(&faces[0])));

    if (AvxLoadRasters(dsys, 1, &info2, &urib.uri, NIL, &ras)) AfxThrowError();
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

        avxRasterIo op = { 0 };
        op.rgn.whd = AvxGetRasterExtent(ras, info2.lodCnt - 1);
        op.rgn.whd.d = 1;

        for (afxUnit i = 1; i < info2.whd.d; i++) // start at 1; base is already loaded.
        {
            op.rgn.origin.z = i;
            
            AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(AfxGetUriString(dir)), AfxPushString(AfxGetUriString(&faces[i])));

            if (AvxReloadRaster(ras, 1, &op, info2.lodCnt, &urib.uri, 0))
                AfxThrowError();
        }
    }
    AFX_ASSERT(raster);
    *raster = ras;
    return err;
}
