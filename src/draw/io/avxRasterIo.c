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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_RASTER_C
#include "../impl/avxImplementation.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x whd >> lod

AVX afxError _AfxPrintRasterToTarga(avxRaster ras, avxRasterIo const* iop, afxUnit lodCnt, afxStream out, afxUnit portId);
AVX afxError _AfxFetchRasterFromTarga(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit lodCnt, afxStream in, afxUnit portId);

_AVXINL void _AvxSanitizeRasterIo(avxRaster ras, afxSize bufCap, avxRasterIo const* raw, avxRasterIo* san)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterIo struct to be within the bounds of a raster object.
    */

    _AvxSanitizeRasterRegion(ras, &raw->rgn, &san->rgn);

    san->offset = AFX_MIN(raw->offset, bufCap - 1);
    san->rowStride = AFX_MIN(raw->rowStride, bufCap - san->offset);
    san->rowsPerImg = AFX_MIN(raw->rowsPerImg, (san->rowStride ? bufCap / san->rowStride : 0));
}

_AVX afxError AvxUpdateRaster(avxRaster ras, void const* src, afxUnit opCnt, avxRasterIo const ops[], afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(src);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_RAS;
    transfer.dst.ras = ras;
    transfer.src.src = src;

    afxDrawSystem dsys = AfxGetProvider(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

_AVX afxError AvxDumpRaster(avxRaster ras, void* dst, afxUnit opCnt, avxRasterIo const ops[], afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

#if AVX_VALIDATION_ENABLED

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_RAS;
    transfer.src.ras = ras;
    transfer.dst.dst = dst;

    afxDrawSystem dsys = AfxGetProvider(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxPackRaster(avxRaster ras, avxBuffer buf, afxUnit opCnt, avxRasterIo const ops[], afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
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
        AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_RAS;
    transfer.dstFcc = afxFcc_BUF;
    transfer.src.ras = ras;
    transfer.dst.buf = buf;

    afxDrawSystem dsys = AfxGetProvider(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

_AVX afxError AvxUnpackRaster(avxRaster ras, avxBuffer buf, afxUnit opCnt, avxRasterIo const ops[], afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
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
        AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_RAS;
    transfer.srcFcc = afxFcc_BUF;
    transfer.dst.ras = ras;
    transfer.src.buf = buf;

    afxDrawSystem dsys = AfxGetProvider(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxUploadRaster(avxRaster ras, afxStream in, afxUnit opCnt, avxRasterIo const ops[], afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
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
        AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_RAS;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.ras = ras;
    transfer.src.iob = in;

    afxDrawSystem dsys = AfxGetProvider(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

_AVX afxError AvxDownloadRaster(avxRaster ras, afxStream out, afxUnit opCnt, avxRasterIo const ops[], afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
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
        AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(AvxGetRasterExtent(ras, op->rgn.lodIdx), op->rgn.origin, op->rgn.extent);
    }
#endif//AVX_VALIDATION_ENABLED

    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_RAS;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.ras = ras;
    transfer.dst.iob = out;

    afxDrawSystem dsys = AfxGetProvider(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxPrintRaster(avxRaster ras, afxUnit lodCnt, avxRasterIo const* iop, afxUri const* uri, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_WRITEABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReopenFile(file, uri, afxFileFlag_W)) AfxThrowError();
    else
    {
        afxUnit maxLodCnt = ras->lodCnt;
        AFX_ASSERT_RANGE(maxLodCnt, 0, lodCnt);
        lodCnt = lodCnt ? AFX_MIN(lodCnt, maxLodCnt) : maxLodCnt;

        avxRasterIo iopClamped = { 0 };
        iopClamped.rgn.extent = AvxGetRasterExtent(ras, 0);

        if (iop)
        {
            AFX_ASSERT_RANGE(maxLodCnt, iop->rgn.lodIdx, lodCnt);
            afxUnit lodIdxClamped = AFX_MIN(iop->rgn.lodIdx, maxLodCnt - 1);
            lodCnt = AFX_MIN(lodCnt, maxLodCnt - lodIdxClamped);

            avxRange maxWhd, maxXyz;
            maxWhd = AvxGetRasterExtent(ras, lodIdxClamped);
            AFX_ASSERT_RANGE_WHD(maxWhd, iop->rgn.origin, iop->rgn.extent);

            iopClamped.rgn.origin = AvxClampOrigin(iop->rgn.origin, maxWhd);

            avxRange maxRgnWhd;
            maxRgnWhd = AvxSubtractRange(maxWhd, AVX_RANGE(iopClamped.rgn.origin.x, iopClamped.rgn.origin.y, iopClamped.rgn.origin.z));
            iopClamped.rgn.extent = AvxClampRange(iop->rgn.extent, AVX_RANGE(1, 1, 1), maxRgnWhd);

            iopClamped.rgn = iop->rgn;
            iopClamped.rgn.lodIdx = lodIdxClamped;
        }

        avxRasterLayout layout;
        avxFormatDescription pfd;
        AvxQueryRasterLayout(ras, 0, 0, &layout);
        avxFormat fmt = AvxDescribeRasterFormat(ras, exuMask, &pfd);

        afxTargaFlags flags = NIL;

        if (AvxTestRasterFlags(ras, avxRasterFlag_CUBEMAP))
            flags |= afxTargaFlag_CUBEMAP;

        if (AvxTestRasterFlags(ras, avxRasterFlag_3D))
            flags |= afxTargaFlag_3D;

        iopClamped.rowStride = AFX_ALIGNED_SIZE(iopClamped.rgn.extent.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        iopClamped.rowsPerImg = iopClamped.rgn.extent.h;

        if (AfxWriteTargaFile(file, &iopClamped, lodCnt, fmt, (avxRasterFlags) flags, 0)) AfxThrowError();
        else
        {
            // write data for each segment

            avxRasterIo iopTmp = iopClamped;

            for (afxUnit i = 0; i < lodCnt; i++)
            {
                avxRasterLayout lodLayout;
                AvxQueryRasterLayout(ras, i, 0, &lodLayout);

                iopTmp.rgn.lodIdx = i;
                iopTmp.offset = AfxAskStreamPosn(file);
                iopTmp.rowStride = iopClamped.rowStride;// lodLayout.rowStride;
                iopTmp.rowsPerImg = iopClamped.rowsPerImg;

                if (AvxDownloadRaster(ras, file, 1, &iopTmp, exuMask))
                    AfxThrowError();

                iopTmp.rgn.origin = AvxHalfOrigin(iopTmp.rgn.origin);
                iopTmp.rgn.extent = AvxHalfRange(iopTmp.rgn.extent);
            }
        }

        afxDrawSystem dsys = AfxGetProvider(ras);
        AvxWaitForDrawBridges(dsys, exuMask, AFX_TIME_INFINITE); // we need to wait for completation before releasing the stream.
    }
    AfxDisposeObjects(1, &file);
    return err;
}

_AVX afxError AvxFetchRaster(avxRaster ras, afxUri const* uri, afxUnit opCnt, avxRasterIo const ops[], afxUnit lodCnt, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
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
    
    avxTargaFile tgai = { 0 };
    if (AfxReadTargaFile(&tgai, file)) AfxThrowError();
    else
    {
        afxDrawSystem dsys = AfxGetProvider(ras);
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

        avxRasterLayout layout;
        AvxQueryRasterLayout(ras, 0, 0, &layout);
        avxFormat fmt = AvxGetRasterFormat(ras);

        afxUnit maxLodCnt = ras->lodCnt;
        AFX_ASSERT_RANGE(maxLodCnt, 0, lodCnt);
        lodCnt = AFX_MIN(AFX_MAX(1, lodCnt), maxLodCnt);

        avxRasterIo iopClamped = { 0 };
        iopClamped.rgn.extent = AvxGetRasterExtent(ras, 0);

        if (opCnt)
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
                AFX_ASSERT_RANGE_WHD(maxWhd, iop->rgn.origin, iop->rgn.extent);

                iopClamped.rgn.origin = AvxClampOrigin(iop->rgn.origin, maxWhd);

                avxRange maxRgnWhd;
                maxRgnWhd = AvxSubtractRange(maxWhd, AVX_RANGE(iopClamped.rgn.origin.x, iopClamped.rgn.origin.y, iopClamped.rgn.origin.z));
                iopClamped.rgn.extent = AvxClampRange(iop->rgn.extent, AVX_RANGE(1, 1, 1), maxRgnWhd);

                iopClamped.rgn = iop->rgn;
                iopClamped.rgn.lodIdx = lodIdxClamped;

                void* data;
                AfxAllocate(tgai.decSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&data);

                AfxDecodeTargaFile(&tgai, file, data);
                AvxUpdateRaster(ras, data, 1, &iopClamped, exuMask);

                AvxWaitForDrawBridges(AfxGetProvider(ras), exuMask, AFX_TIME_INFINITE);

                AfxDeallocate((void**)&data, AfxHere());
            }
        }
        else
        {
            avxRasterIo op = { 0 };
            op.rgn.extent.w = tgai.width;
            op.rgn.extent.h = tgai.height;
            op.rgn.extent.d = tgai.depth;
            op.offset = 0;
            op.rowStride = tgai.rowStride;
            op.rowsPerImg = tgai.height;

            void* data;
            AfxAllocate(tgai.decSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&data);

            AfxDecodeTargaFile(&tgai, file, data);
            AvxUpdateRaster(ras, data, 1, &op, exuMask);

            AvxWaitForDrawBridges(AfxGetProvider(ras), exuMask, AFX_TIME_INFINITE);

            AfxDeallocate((void**)&data, AfxHere());
        }

        //AvxWaitForDrawBridges(dsys, portId, AFX_TIME_INFINITE); // we need to wait for completation before releasing the stream.
    }
    AfxDisposeObjects(1, &file);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxLoadRasters(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const info[], afxUri const uri[], avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
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

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (AfxReopenFile(file, &uri[i], afxFileFlag_R)) AfxThrowError();
        else
        {
            avxTargaFile tgai = { 0 };

            if (AfxReadTargaFile(&tgai, file))
            {
                AfxThrowError();
                rasters[i] = NIL;
            }
            else
            {
                avxFormat fmt = tgai.fmt;

                avxRasterInfo rasi = { 0 };

                if (info)
                {
                    rasi.fmt = info[i].fmt ? info[i].fmt : fmt;
                    rasi.lodCnt = info[i].lodCnt;
                    rasi.usage = info[i].usage;
                    rasi.flags = info[i].flags;

                    rasi.extent.w = AFX_MAX(1, AFX_MAX(info[i].extent.w, tgai.width));
                    rasi.extent.h = AFX_MAX(1, AFX_MAX(info[i].extent.h, tgai.height));
                    rasi.extent.d = AFX_MAX(1, AFX_MAX(info[i].extent.d, tgai.depth));

                    rasi.flags = info[i].flags | (tgai.flags & afxTargaFlag_CUBEMAP ? avxRasterFlag_CUBEMAP : NIL);
                    rasi.usage = info[i].usage ? info[i].usage : avxRasterUsage_RESAMPLE | avxRasterUsage_SRC;
                }
                else
                {
                    rasi.fmt = tgai.fmt;
                    rasi.lodCnt = tgai.lodCnt;
                    rasi.flags = tgai.flags & afxTargaFlag_CUBEMAP ? avxRasterFlag_CUBEMAP : NIL;
                    rasi.usage = avxRasterUsage_RESAMPLE | avxRasterUsage_SRC;

                    rasi.extent.w = tgai.width;
                    rasi.extent.h = tgai.height;
                    rasi.extent.d = tgai.depth;
                }

                if (AvxAcquireRasters(dsys, 1, &rasi, &rasters[i]))
                {
                    AfxThrowError();
                    AfxDisposeObjects(i, (void**)rasters);
                }
                else
                {
                    avxRasterIo op = { 0 };
                    op.rgn.extent.w = tgai.width;
                    op.rgn.extent.h = tgai.height;
                    op.rgn.extent.d = tgai.depth;
                    op.offset = 0;
                    op.rowStride = tgai.rowStride;
                    op.rowsPerImg = tgai.rowsPerImg;
                    //op.decSiz = tgai.decSiz;

                    void* data = AfxRequestArenaUnit(&arena, tgai.decSiz, 1, NIL, 0);

                    AfxDecodeTargaFile(&tgai, file, data);
                    AvxUpdateRaster(rasters[i], data, 1, &op, portId);
                }
            }
        }
    }

    AvxWaitForDrawBridges(dsys, portId, AFX_TIME_INFINITE);

    AfxDismantleArena(&arena);

    if (file)
        AfxDisposeObjects(1, &file);

    return err;
}

_AVX avxRaster AvxLoadCubemapRaster(afxDrawSystem dsys, avxRasterInfo const* info, afxUri const* dir, afxUri const faces[6])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(faces);
    avxRaster ras;

    avxRasterInfo info2 = *info;
    info2.flags |= avxRasterFlag_CUBEMAP;
    info2.usage |= avxRasterUsage_RESAMPLE;
    info2.extent.d = 6;

    afxUri2048 urib;
    AfxMakeUri2048(&urib, NIL);
    AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(AfxGetUriString(dir)), AfxPushString(AfxGetUriString(&faces[0])));

    if (AvxLoadRasters(dsys, 1, &info2, &urib.uri, &ras)) AfxThrowError();
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

        avxRasterIo op = { 0 };
        op.rgn.extent = AvxGetRasterExtent(ras, info2.lodCnt);
        op.rgn.extent.d = 1;

        for (afxUnit i = 1; i < 6; i++) // start at 1; base is already loaded.
        {
            op.rgn.origin.z = i;
            
            AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(AfxGetUriString(dir)), AfxPushString(AfxGetUriString(&faces[i])));

            if (AvxFetchRaster(ras, &urib.uri, 1, &op, info2.lodCnt, 0))
                AfxThrowError();
        }
    }
    return ras;
}
