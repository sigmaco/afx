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

_AVXINL avxFormat AvxGetRasterFormat(avxRaster ras)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif
    return ras->fmt;
}

_AVXINL avxFormat AvxDescribeRasterFormat(avxRaster ras, afxUnit exuIdx, avxFormatDescription* pfd)
{
    afxError err = AFX_ERR_NONE;
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_S(pfd, "must be a valid pointer to a avxFormatDescription structure.");
#endif//AVX_VALIDATION_ENABLED

    // This function is a utility to ease introdution of device-dependent formats.
    // Give the multi-device nature of Qwadro, it takes a index of a bridge established in the provider system.

    afxDrawSystem dsys = AfxGetProvider(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxDrawBridge dexu;
    AvxGetDrawBridges(dsys, exuIdx, 1, &dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    afxDrawDevice ddev = AvxGetDrawBridgeDevice(dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    avxFormat fmt = ras->fmt;
    avxFormatDescription desc;
    if (AvxDescribeDeviceFormats(ddev, 1, &fmt, &desc))
    {
        AfxThrowError();
        *pfd = (avxFormatDescription) { 0 };
        return NIL;
    }
    else
    {
        *pfd = desc;
        return fmt;
    }
}

_AVXINL avxRasterUsage AvxTestRasterUsage(avxRaster ras, avxRasterUsage mask)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif//AVX_VALIDATION_ENABLED
    return ras->usage & mask;
}

_AVXINL avxRasterFlags AvxTestRasterFlags(avxRaster ras, avxRasterFlags mask)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif//AVX_VALIDATION_ENABLED
    return ras->flags & mask;
}

_AVXINL void AvxGetRasterSwizzling(avxRaster ras, avxSwizzling* csw)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(csw);
#endif//AVX_VALIDATION_ENABLED
    *csw = ras->swizzling;
}

_AVXINL avxRange AvxGetRasterExtent(avxRaster ras, afxUnit lodIdx)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_RANGE(ras->lodCnt, lodIdx, 1);
#endif//AVX_VALIDATION_ENABLED

    avxRange whd = ras->extent;
    if (AvxTestRasterFlags(ras, avxRasterFlag_UPSAMPLED))
    {
        while (lodIdx--)
        {
            whd.w = whd.w * 2;
            whd.h = whd.h * 2;
            whd.d = whd.d * 2;
        }
    }
    else
    {
        while (lodIdx--)
        {
            whd.w = whd.w >> 1;
            whd.h = whd.h >> 1;
            whd.d = whd.d >> 1;
        }
        whd = AvxMaxRange(AVX_RANGE(1, 1, 1), whd);
    }
    return whd;
}

_AVXINL void AvxDescribeRaster(avxRaster ras, avxRasterInfo* desc)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(desc);
#endif//AVX_VALIDATION_ENABLED
    desc->lodCnt = ras->lodCnt;
    desc->extent = AvxGetRasterExtent(ras, 0);
    desc->fmt = ras->fmt;
    desc->flags = ras->flags;
    desc->usage = ras->usage;
    desc->base = ras->base;
    desc->baseLayer = ras->baseLayer;
    desc->baseLod = ras->baseLod;
    desc->exuIdx = 0;
    desc->swizzle = ras->swizzling;
    desc->udd = ras->udd;
    desc->tag = ras->tag;
}

_AVXINL void _AvxSanitizeRasterRegion(avxRaster ras, avxRasterRegion const* raw, avxRasterRegion* san)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterRegion struct to be within the bounds of a raster object.
    */

    san->lodIdx = AfxMin(raw->lodIdx, ras->lodCnt - 1);
    san->origin.x = AfxMin(raw->origin.x, (afxInt)(ras->extent.w - 1));
    san->origin.y = AfxMin(raw->origin.y, (afxInt)(ras->extent.h - 1));
    san->origin.z = AfxMin(raw->origin.z, (afxInt)(ras->extent.d - 1));
    san->extent.w = AfxClamp(raw->extent.w, 1, ras->extent.w - san->origin.x);
    san->extent.h = AfxClamp(raw->extent.h, 1, ras->extent.h - san->origin.y);
    san->extent.d = AfxClamp(raw->extent.d, 1, ras->extent.d - san->origin.z);
}

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

    san->offset = AfxMin(raw->offset, bufCap - 1);
    san->rowStride = AfxMin(raw->rowStride, bufCap - san->offset);
    san->rowsPerImg = AfxMin(raw->rowsPerImg, (san->rowStride ? bufCap / san->rowStride : 0));
}

_AVXINL void _AvxSanitizeRasterCopy(avxRaster src, avxRaster dst, avxRasterCopy const* raw, avxRasterCopy* san)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // src must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);
    // dst must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterCopy struct to be within the bounds of two raster objects.
    */

    san->src.lodIdx = AfxMin(raw->src.lodIdx, src->lodCnt - 1);
    san->src.origin.x = AfxMin(raw->src.origin.x, (afxInt)(src->extent.w - 1));
    san->src.origin.y = AfxMin(raw->src.origin.y, (afxInt)(src->extent.h - 1));
    san->src.origin.z = AfxMin(raw->src.origin.z, (afxInt)(src->extent.d - 1));

    san->dstLodIdx = AfxMin(raw->dstLodIdx, dst->lodCnt - 1);
    san->dstOrigin.x = AfxMin(raw->dstOrigin.x, (afxInt)(dst->extent.w - 1));
    san->dstOrigin.y = AfxMin(raw->dstOrigin.y, (afxInt)(dst->extent.h - 1));
    san->dstOrigin.z = AfxMin(raw->dstOrigin.z, (afxInt)(dst->extent.d - 1));

    san->src.extent.w = AfxClamp(raw->src.extent.w, 1, AfxMin((afxInt)(src->extent.w - san->src.origin.x), (afxInt)(dst->extent.w - san->dstOrigin.x)));
    san->src.extent.h = AfxClamp(raw->src.extent.h, 1, AfxMin((afxInt)(src->extent.h - san->src.origin.y), (afxInt)(dst->extent.h - san->dstOrigin.y)));
    san->src.extent.d = AfxClamp(raw->src.extent.d, 1, AfxMin((afxInt)(src->extent.d - san->src.origin.z), (afxInt)(dst->extent.d - san->dstOrigin.z)));
}

_AVX afxBool AvxQueryRasterLayout(avxRaster ras, afxUnit lodIdx, afxUnit layerIdx, avxRasterLayout* layout)
{
    afxError err = AFX_ERR_NONE;
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_RANGE(ras->lodCnt, lodIdx, 1);
    AFX_ASSERT_RANGE(ras->extent.d, layerIdx, 1);
#endif//AVX_VALIDATION_ENABLED

    afxBool is3d = AvxTestRasterFlags(ras, avxRasterFlag_3D);
    AFX_ASSERT(!layerIdx || !is3d);

    avxRange whd = AvxGetRasterExtent(ras, lodIdx);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &ras->fmt, &pfd);
    
    afxUnit rowStride = AFX_ALIGNED_SIZE(pfd.stride * whd.w, AFX_SIMD_ALIGNMENT);
    afxUnit imgStride = whd.h * rowStride;
    AFX_ASSERT(layout);
    layout->imgStride = imgStride;
    layout->rowStride = rowStride;
    layout->offset = 0;
    layout->size = is3d ? whd.d * imgStride : whd.d * imgStride;
    return !err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxUpdateRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit exuIdx, void const* src)
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
    transfer.exuIdx = exuIdx;
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

_AVX afxError AvxDumpRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit exuIdx, void* dst)
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
    transfer.exuIdx = exuIdx;
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

_AVX afxError AvxPackRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit exuIdx, avxBuffer buf)
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
    transfer.exuIdx = exuIdx;
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

_AVX afxError AvxUnpackRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit exuIdx, avxBuffer buf)
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
    transfer.exuIdx = exuIdx;
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

_AVX afxError AvxUploadRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit exuIdx, afxStream in)
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
    transfer.exuIdx = exuIdx;
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

_AVX afxError AvxDownloadRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit exuIdx, afxStream out)
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
    transfer.exuIdx = exuIdx;
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

_AVX afxError AvxPrintRaster(avxRaster ras, avxRasterIo const* iop, afxUnit lodCnt, afxUnit exuIdx, afxUri const* uri)
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
        lodCnt = lodCnt ? AfxMin(lodCnt, maxLodCnt) : maxLodCnt;

        avxRasterIo iopClamped = { 0 };
        iopClamped.rgn.extent = AvxGetRasterExtent(ras, 0);

        if (iop)
        {
            AFX_ASSERT_RANGE(maxLodCnt, iop->rgn.lodIdx, lodCnt);
            afxUnit lodIdxClamped = AfxMin(iop->rgn.lodIdx, maxLodCnt - 1);
            lodCnt = AfxMin(lodCnt, maxLodCnt - lodIdxClamped);

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
        avxFormat fmt = AvxDescribeRasterFormat(ras, exuIdx, &pfd);

        afxTargaFlags flags = NIL;

        if (AvxTestRasterFlags(ras, avxRasterFlag_CUBEMAP))
            flags |= afxTargaFlag_CUBEMAP;

        if (AvxTestRasterFlags(ras, avxRasterFlag_3D))
            flags |= afxTargaFlag_3D;

        iopClamped.rowStride = AFX_ALIGNED_SIZE(iopClamped.rgn.extent.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        iopClamped.rowsPerImg = iopClamped.rgn.extent.h;

        if (AfxWriteRasterFile(file, &iopClamped, lodCnt, fmt, (avxRasterFlags) flags, 0)) AfxThrowError();
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

                if (AvxDownloadRaster(ras, 1, &iopTmp, exuIdx, file))
                    AfxThrowError();

                iopTmp.rgn.origin = AvxHalfOrigin(iopTmp.rgn.origin);
                iopTmp.rgn.extent = AvxHalfRange(iopTmp.rgn.extent);
            }
        }

        afxDrawSystem dsys = AfxGetProvider(ras);
        AvxWaitForDrawBridge(dsys, exuIdx, AFX_TIME_INFINITE); // we need to wait for completation before releasing the stream.
    }
    AfxDisposeObjects(1, &file);
    return err;
}

_AVX afxError AvxFetchRaster(avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxUnit lodCnt, afxUnit exuIdx, afxUri const* uri)
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
    
    afxRasterFile tgai = { 0 };
    if (AfxReadRasterFile(&tgai, file)) AfxThrowError();
    else
    {
        afxDrawSystem dsys = AfxGetProvider(ras);
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

        avxRasterLayout layout;
        AvxQueryRasterLayout(ras, 0, 0, &layout);
        avxFormat fmt = AvxGetRasterFormat(ras);

        afxUnit maxLodCnt = ras->lodCnt;
        AFX_ASSERT_RANGE(maxLodCnt, 0, lodCnt);
        lodCnt = AfxMin(AfxMax(1, lodCnt), maxLodCnt);

        avxRasterIo iopClamped = { 0 };
        iopClamped.rgn.extent = AvxGetRasterExtent(ras, 0);

        if (opCnt)
        {
            AFX_ASSERT(ops);

            for (afxUnit i = 0; i < opCnt; i++)
            {
                avxRasterIo const* iop = &ops[i];

                AFX_ASSERT_RANGE(maxLodCnt, iop->rgn.lodIdx, lodCnt);
                afxUnit lodIdxClamped = AfxMin(iop->rgn.lodIdx, maxLodCnt - 1);
                lodCnt = AfxMin(lodCnt, maxLodCnt - lodIdxClamped);

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

                AfxDecodeRasterFile(&tgai, file, data);
                AvxUpdateRaster(ras, 1, &iopClamped, exuIdx, data);

                AvxWaitForDrawBridge(AfxGetProvider(ras), exuIdx, AFX_TIME_INFINITE);

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

            AfxDecodeRasterFile(&tgai, file, data);
            AvxUpdateRaster(ras, 1, &op, exuIdx, data);

            AvxWaitForDrawBridge(AfxGetProvider(ras), exuIdx, AFX_TIME_INFINITE);

            AfxDeallocate((void**)&data, AfxHere());
        }

        //AvxWaitForDrawBridge(dsys, portId, AFX_TIME_INFINITE); // we need to wait for completation before releasing the stream.
    }
    AfxDisposeObjects(1, &file);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxRasDtorCb(avxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    if (ras->base)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras->base);
        AfxDisposeObjects(1, &ras->base);
    }

    return err;
}

_AVX afxError _AvxRasCtorCb(avxRaster ras, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxDrawSystem dsys = AfxGetProvider(ras);
    avxRasterInfo const* rasi = ((avxRasterInfo const *)args[1]) + invokeNo;

    ras->tag = rasi->tag;
    ras->base = NIL;

    //ras->swizzling = NIL; //&AVX_COLOR_SWIZZLING_IDENTITY;

    ras->baseLod = 0;
    ras->lodCnt = AfxMax(rasi->lodCnt, 1);
    ras->baseLayer = 0;
    ras->extent = AvxMaxRange(AVX_RANGE(1, 1, 1), rasi->extent);

    ras->flags = NIL;
    ras->usage = rasi->usage;

    if (!rasi->fmt)
    {
        // must have a specified format.
        AfxThrowError();
        return err;
    }
    ras->fmt = rasi->fmt;

    ras->swizzling.r = rasi->swizzle.r;
    ras->swizzling.g = rasi->swizzle.g;
    ras->swizzling.b = rasi->swizzle.b;
    ras->swizzling.a = rasi->swizzle.a;

    ras->udd = rasi->udd;

    // FLAGS

    avxRasterFlags const mipAndMs = (avxRasterFlag_MIPMAP | avxRasterFlag_UPSAMPLED);
    avxRasterFlags flags = (rasi->flags & mipAndMs);

    if (flags == mipAndMs)
    {
        // can not be mipmapped and multisampled at same time.
        AfxThrowError();
        return err;
    }
    else if ((flags == NIL) && (ras->lodCnt > 1))
    {
        // If not specified, default to mipmap
        ras->flags |= avxRasterFlag_MIPMAP;
    }
    else
    {
        ras->flags |= (flags & mipAndMs);
    }
    
    flags = (rasi->flags & (avxRasterFlag_3D | avxRasterFlag_CUBEMAP | avxRasterFlag_LAYERED));

    if ((flags == (avxRasterFlag_3D | avxRasterFlag_LAYERED)) &&
        (flags == (avxRasterFlag_3D | avxRasterFlag_CUBEMAP)))
    {
        // can not be volumetric (3D) and layered or cubemap at same time.
        AfxThrowError();
        return err;
    }
    else if ((flags == NIL) && (ras->extent.d > 1))
    {
        // If not specified, default to layered.
        ras->flags |= avxRasterFlag_LAYERED;
    }
    else
    {
        ras->flags |= (flags & (avxRasterFlag_3D | avxRasterFlag_CUBEMAP | avxRasterFlag_LAYERED));

        if (flags & avxRasterFlag_CUBEMAP)
        {
            // if cubemap, layers must be a multiple of 6.

            if (ras->extent.d % 6)
            {
                AfxThrowError();
                return err;
            }
        }
    }

    flags = (rasi->flags & (avxRasterFlag_1D | avxRasterFlag_2D | avxRasterFlag_3D));

    if ((flags == (avxRasterFlag_2D | avxRasterFlag_1D)) ||
        (flags == (avxRasterFlag_2D | avxRasterFlag_3D)) ||
        (flags == (avxRasterFlag_3D | avxRasterFlag_1D)))
    {
        // Can not be both at same time.
        AfxThrowError();
        return err;
    }
    else if (flags == NIL)
    {
        // default to 2D if nothing is specified.
        ras->flags |= avxRasterFlag_2D;
        // default to layered if depth is present. See code above.
    }
    else
    {
        ras->flags |= flags;
    }

    // STORAGE

    {
        avxFormatDescription pfd;
        AvxDescribeFormats(1, &ras->fmt, &pfd);

        afxUnit align = AVX_RASTER_ALIGNMENT;
        afxUnit size = 0;

        afxWhd whd = { ras->extent.w, ras->extent.h, ras->extent.d };
        afxBool is3d = !!AvxTestRasterFlags(ras, avxRasterFlag_3D);

        if (AvxTestRasterFlags(ras, avxRasterFlag_UPSAMPLED))
        {
            for (afxUnit i = 0; i < ras->lodCnt; i++)
            {
                afxUnit rowStride = AFX_ALIGNED_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
                afxUnit imgStride = AFX_ALIGNED_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
                size += whd.d * imgStride;

                whd.w = AfxMax(1, whd.w * 2);
                whd.h = AfxMax(1, whd.h * 2);
                if (is3d) whd.d = AfxMax(1, whd.d * 2);
            }
        }
        else
        {
            for (afxUnit i = 0; i < ras->lodCnt; i++)
            {
                afxUnit rowStride = AFX_ALIGNED_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
                afxUnit imgStride = AFX_ALIGNED_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
                size += whd.d * imgStride;

                whd.w = AfxMax(1, whd.w >> 1);
                whd.h = AfxMax(1, whd.h >> 1);
                if (is3d) whd.d = AfxMax(1, whd.d >> 1);
            }
        }

        size = AFX_ALIGNED_SIZE(size, align);

        ras->storage[0].size = size;
        ras->storage[0].alignment = align;
        ras->storage[0].memType = NIL;

        ras->storage[0].mmu = AFX_INVALID_INDEX;
        ras->storage[0].offset = 0;
        ras->storage[0].hostedData.addr = NIL;
    }
    return err;
}

_AVX afxClassConfig const _AVX_RAS_CLASS_CONFIG =
{
    .fcc = afxFcc_RAS,
    .name = "Raster",
    .desc = "Formatted Video Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(avxRaster)),
    .ctor = (void*)_AvxRasCtorCb,
    .dtor = (void*)_AvxRasDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireRasters(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const info[], avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(rasters);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterFlags flags = info[i].flags;

        if ((flags & avxRasterFlag_CUBEMAP) && (info[i].extent.d % 6))
        {
            AFX_ASSERT(info[i].extent.d % 6 == 0);
            AfxThrowError();

            if ((info[i].extent.d / 6) && !(flags & avxRasterFlag_LAYERED))
            {
                AFX_ASSERT_AND((info[i].extent.d / 6), (flags & avxRasterFlag_LAYERED));
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_1D) && (info[i].extent.h > 1))
        {
            AFX_ASSERT(1 >= info[i].extent.h);
            AfxThrowError();

            if ((info[i].extent.d > 1) && !(flags & avxRasterFlag_LAYERED))
            {
                AFX_ASSERT_OR(1 >= info[i].extent.d, flags & avxRasterFlag_LAYERED);
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_2D) && (info[i].extent.d > 1) && !(flags & avxRasterFlag_LAYERED))
        {
            AfxThrowError();

            if ((info[i].extent.d > 1) && !(flags & avxRasterFlag_LAYERED))
            {
                AFX_ASSERT_OR(1 >= info[i].extent.d, flags & avxRasterFlag_LAYERED);
                AfxThrowError();
            }
        }
    }
#endif//AVX_VALIDATION_ENABLED

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->rasCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dsys, info }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterInfo rasi;
        AvxDescribeRaster(rasters[i], &rasi);

        AFX_ASSERT(rasi.base == info[i].base);
        AFX_ASSERT(rasi.lodCnt >= info[i].lodCnt);
        AFX_ASSERT(rasi.baseLayer == info[i].baseLayer);
        AFX_ASSERT(rasi.baseLod == info[i].baseLod);
        AFX_ASSERT(rasi.extent.w >= info[i].extent.w);
        AFX_ASSERT(rasi.extent.h >= info[i].extent.h);
        AFX_ASSERT(rasi.extent.d >= info[i].extent.d);
        AFX_ASSERT(rasi.exuIdx == info[i].exuIdx);
        AFX_ASSERT((rasi.flags & info[i].flags) == info[i].flags);
        AFX_ASSERT(rasi.fmt == info[i].fmt);
        AFX_ASSERT(rasi.tag.start == info[i].tag.start);
        AFX_ASSERT(rasi.swizzle.r == info[i].swizzle.r);
        AFX_ASSERT(rasi.swizzle.g == info[i].swizzle.g);
        AFX_ASSERT(rasi.swizzle.b == info[i].swizzle.b);
        AFX_ASSERT(rasi.swizzle.a == info[i].swizzle.a);
        AFX_ASSERT(rasi.udd == info[i].udd);
        AFX_ASSERT((rasi.usage & info[i].usage) == info[i].usage);
    }
#endif

    return err;
}

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
    AfxDeployArena(&arena, NIL, AfxHere());

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (AfxReopenFile(file, &uri[i], afxFileFlag_R)) AfxThrowError();
        else
        {
            afxRasterFile tgai = { 0 };

            if (AfxReadRasterFile(&tgai, file))
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

                    rasi.extent.w = AfxMax(1, AfxMax(info[i].extent.w, tgai.width));
                    rasi.extent.h = AfxMax(1, AfxMax(info[i].extent.h, tgai.height));
                    rasi.extent.d = AfxMax(1, AfxMax(info[i].extent.d, tgai.depth));

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

                    void* data = AfxRequestArenaUnit(&arena, tgai.decSiz);

                    AfxDecodeRasterFile(&tgai, file, data);
                    AvxUpdateRaster(rasters[i], 1, &op, portId, data);
                }
            }
        }
    }

    AvxWaitForDrawBridge(dsys, portId, AFX_TIME_INFINITE);

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

            if (AvxFetchRaster(ras, 1, &op, info2.lodCnt, 0, &urib.uri))
                AfxThrowError();
        }
    }
    return ras;
}
