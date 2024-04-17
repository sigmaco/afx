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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_DRAW_C
#define _AFX_RASTER_C
#include "qwadro/draw/afxDrawSystem.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x whd >> lod

_AVXINL afxDrawContext AfxGetRasterContext(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxDrawContext dctx = AfxGetObjectProvider(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVXINL afxNat AfxCountRasterSamples(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->sampleCnt;
}

_AVXINL afxPixelFormat AfxGetRasterFormat(afxRaster ras)
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

_AVXINL afxNat AfxCountRasterLods(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->lodCnt;
}

_AVXINL afxNat AfxCountRasterLayers(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->layerCnt;
}

_AVXINL afxBool AfxGetRasterSwizzling(afxRaster ras, afxColorSwizzling const** csw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    if (csw)
        *csw = ras->swizzling;

    return !!ras->swizzling;
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
}

_AVXINL void AfxDescribeRaster(afxRaster ras, afxRasterInfo* desc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(desc);
    desc->lodCnt = ras->lodCnt;
    desc->layerCnt = ras->layerCnt;
    desc->sampleCnt = ras->sampleCnt;
    desc->whd[0] = ras->whd[0];
    desc->whd[1] = ras->whd[1];
    desc->whd[2] = ras->whd[2];
    desc->fmt = ras->fmt;
    desc->flags = ras->flags;
    desc->usage = ras->usage;
}

_AVXINL void AfxDetermineRasterStride(afxRaster ras, afxNat lodIdx, afxNat* bytesPerRow, afxNat* bytesPerLayer)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);

    afxNat w = ras->whd[0], h = ras->whd[1], d = ras->whd[2];
    while (lodIdx--) w = w >> 1, h = h >> 1, d = d >> 1;
    AfxAssert(w);

    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat bpr = (w * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE;
    AfxAssert(bpr); // bytes per row
    afxNat bpl = bpr * h * d;
    AfxAssert(bpl); // bytes per layer
    
    AfxAssert(bytesPerRow);
    *bytesPerRow = bpr;
    AfxAssert(bytesPerLayer);
    *bytesPerLayer = bpl;
}

_AVX afxNat AfxDetermineRasterOffset(afxRaster ras, afxNat lodIdx, afxNat layerIdx, afxWhd const offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssertRange(ras->layerCnt, layerIdx, 1);
    //AfxAssertRangef(ras->whd[0], offset[0], 1); // is a evil trick to use it to measure ras
    //AfxAssertRangef(ras->whd[1], offset[1], 1);
    //AfxAssertRangef(ras->whd[2], offset[2], 1);
    
    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat rowSiz, layerSiz;
    AfxDetermineRasterStride(ras, lodIdx, &rowSiz, &layerSiz);

    afxWhd whd;
    AfxGetRasterExtent(ras, lodIdx, whd);
    afxNat memOff = 0;
    afxWhd offset2 = { offset[0], offset[1], offset[2] };

    while (lodIdx)
    {
        offset2[0] = offset2[0] >> 1;
        offset2[1] = offset2[1] >> 1;
        offset2[2] = offset2[2] >> 1;
        //rowSiz = rowSiz >> 1;
        //whd[0] = whd[0] >> 1;
        //whd[1] = whd[1] >> 1;
        //whd[2] = whd[2] >> 1;
        
        memOff += ras->layerCnt * whd[2] * whd[1] * rowSiz;
        --lodIdx;
    }

    memOff += layerIdx * whd[2] * whd[1] * rowSiz;
    memOff += offset2[2] * offset2[1] * ((offset2[0] * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE);
    //memOff = AFX_ALIGN(memOff, AFX_SIMD_ALIGN);

    return memOff;
}

_AVX afxNat AfxMeasureRasterRegion(afxRaster ras, afxRasterRegion const *srcRgn) // can't be multiple because regions couldn't be continous.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(srcRgn);
    AfxAssertRange(ras->lodCnt, srcRgn->lodIdx, 1);
    AfxAssertRange(ras->layerCnt, srcRgn->baseLayer, srcRgn->layerCnt);
    AfxAssertRange(ras->whd[0], srcRgn->origin[0], srcRgn->whd[0]);
    AfxAssertRange(ras->whd[1], srcRgn->origin[1], srcRgn->whd[1]);
    AfxAssertRange(ras->whd[2], srcRgn->origin[2], srcRgn->whd[2]);

    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);

    afxNat lodIdx = srcRgn->lodIdx;
    afxNat rowSiz, layerSiz;
    AfxDetermineRasterStride(ras, lodIdx, &rowSiz, &layerSiz);

    afxWhd offset2 = { srcRgn->origin[0], srcRgn->origin[1], srcRgn->origin[2] };
    afxWhd extent2 = { srcRgn->whd[0], srcRgn->whd[1], srcRgn->whd[2] };
    afxNat size = 0;

    afxWhd whd;
    AfxGetRasterExtent(ras, lodIdx, whd);
    
    while (lodIdx)
    {
        offset2[0] = offset2[0] >> 1;
        offset2[1] = offset2[1] >> 1;
        offset2[2] = offset2[2] >> 1;
        extent2[0] = extent2[0] >> 1;
        extent2[1] = extent2[1] >> 1;
        extent2[2] = extent2[2] >> 1;
        //rowSiz = rowSiz >> 1;
        //whd[0] = whd[0] >> 1;
        //whd[1] = whd[1] >> 1;
        //whd[2] = whd[2] >> 1;

        //memOff += (ras->layerCnt * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
        --lodIdx;
    }
    
    extent2[0] = AfxMax(extent2[0], 1);
    extent2[1] = AfxMax(extent2[1], 1);
    extent2[2] = AfxMax(extent2[2], 1);

    for (afxNat i = srcRgn->layerCnt; i--> 1;)
        size += (whd[2] * whd[1] * rowSiz);

    size += extent2[2] * extent2[1] * ((extent2[0] * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE);

    size -= offset2[2] * offset2[1] * ((offset2[0] * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE);

    size = AFX_ALIGN(size, AFX_SIMD_ALIGN);

    return size;
}

////////////////////////////////////////////////////////////////////////////////
// SERIALIZATION AND STORAGE                                                  //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUploadRaster(afxRaster ras, afxRasterIoOp const* op, afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(op);
    
    afxRasterRegion rgn = op->rgn;
    AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn.baseLayer, rgn.layerCnt);
    AfxAssertRange(ras->whd[0], rgn.origin[0], rgn.whd[0]);
    AfxAssertRange(ras->whd[1], rgn.origin[1], rgn.whd[1]);
    AfxAssertRange(ras->whd[2], rgn.origin[2], rgn.whd[2]);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxTransferRequest req = { 0 };
    req.img.ras = ras;
    req.img.rgn = op->rgn;
    req.dstFcc = afxFcc_RAS;
    req.img.iob = in;
    req.srcFcc = afxFcc_IOB;
    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(ddge, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxDownloadRaster(afxRaster ras, afxRasterIoOp const* op, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(op);

    afxRasterRegion rgn = op->rgn;
    AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn.baseLayer, rgn.layerCnt);
    AfxAssertRange(ras->whd[0], rgn.origin[0], rgn.whd[0]);
    AfxAssertRange(ras->whd[1], rgn.origin[1], rgn.whd[1]);
    AfxAssertRange(ras->whd[2], rgn.origin[2], rgn.whd[2]);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxTransferRequest req = { 0 };
    req.img.ras = ras;
    req.img.rgn = op->rgn;
    req.srcFcc = afxFcc_RAS;
    req.img.iob = out;
    req.dstFcc = afxFcc_IOB;
    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(ddge, queIdx))
            AfxThrowError();
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUpdateRaster(afxRaster ras, afxRasterRegion const* rgn, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssertRange(ras->lodCnt, rgn->lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->origin[0], rgn->whd[0]);
    AfxAssertRange(ras->whd[1], rgn->origin[1], rgn->whd[1]);
    AfxAssertRange(ras->whd[2], rgn->origin[2], rgn->whd[2]);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxTransferRequest req = { 0 };
    req.img.ras = ras;
    req.img.rgn = *rgn;
    req.dstFcc = afxFcc_RAS;
    req.img.src = src;
    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(ddge, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxDumpRaster(afxRaster ras, afxRasterRegion const* rgn, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssertRange(ras->lodCnt, rgn->lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->origin[0], rgn->whd[0]);
    AfxAssertRange(ras->whd[1], rgn->origin[1], rgn->whd[1]);
    AfxAssertRange(ras->whd[2], rgn->origin[2], rgn->whd[2]);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxTransferRequest req = { 0 };
    req.img.ras = ras;
    req.img.rgn = *rgn;
    req.srcFcc = afxFcc_RAS;
    req.img.dst = dst;
    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(ddge, queIdx))
            AfxThrowError();
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxEnumerateRasters(afxDrawContext dctx, afxNat first, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(rasters);
    AfxAssert(cnt);
    afxManager* cls = AfxGetRasterClass(dctx);
    AfxAssertClass(cls, afxFcc_RAS);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)rasters);
}

_AVX afxError AfxAcquireRasters(afxDrawContext dctx, afxNat cnt, afxRasterInfo const info[], afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(rasters);
    AfxAssert(info);
    AfxAssert(cnt);

    afxManager* cls = AfxGetRasterClass(dctx);
    AfxAssertClass(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dctx, info }))
        AfxThrowError();

    AfxAssertObjects(cnt, rasters, afxFcc_RAS);

    return err;
}

_AVX afxRaster AfxAssembleRaster(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxUri const* dir, afxNat cnt, afxUri const layers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(layers);
    AfxAssert(cnt);
    afxRaster ras;

    if (AfxAssembleRastersFromTarga(dctx, usage, flags, dir, cnt, layers, &ras))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}

_AVX afxRaster AfxAssembleCubemapRasters(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxUri const* dir, afxUri const faces[6])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(faces);
    afxRaster ras;

    if (!(ras = AfxAssembleRaster(dctx, usage, flags | afxRasterFlag_CUBEMAP, dir, 6, faces)))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}
