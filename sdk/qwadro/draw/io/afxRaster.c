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

_AVXINL afxRasterUsage AfxTestRasterUsage(afxRaster ras, afxRasterUsage bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->usage & bitmask;
}

_AVXINL afxRasterFlags AfxGetRasterFlags(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->flags;
}

_AVXINL afxRasterFlags AfxTestRasterFlags(afxRaster ras, afxRasterFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->flags & bitmask;
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

    while (lodIdx)
    {
        whd[0] = whd[0] >> 1;
        whd[1] = whd[1] >> 1;
        whd[2] = whd[2] >> 1;
        --lodIdx;
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

    afxNat w = ras->whd[0];
    while (lodIdx--) w = w >> 1;
    AfxAssert(w);

    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat bpr = (w * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE;
    AfxAssert(bpr); // bytes per row
    afxNat bpl = bpr * ras->whd[1] * ras->whd[2];
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
    AfxAssertRange(ras->whd[0], srcRgn->offset[0], srcRgn->whd[0]);
    AfxAssertRange(ras->whd[1], srcRgn->offset[1], srcRgn->whd[1]);
    AfxAssertRange(ras->whd[2], srcRgn->offset[2], srcRgn->whd[2]);

    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);

    afxNat lodIdx = srcRgn->lodIdx;
    afxNat rowSiz, layerSiz;
    AfxDetermineRasterStride(ras, lodIdx, &rowSiz, &layerSiz);

    afxWhd offset2 = { srcRgn->offset[0], srcRgn->offset[1], srcRgn->offset[2] };
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
    AfxAssertRange(ras->whd[0], rgn.offset[0], rgn.whd[0]);
    AfxAssertRange(ras->whd[1], rgn.offset[1], rgn.whd[1]);
    AfxAssertRange(ras->whd[2], rgn.offset[2], rgn.whd[2]);

    afxNat rgnSiz = 0;
    afxNat dstRowSiz = 0;
    void *dstData;
        
    if (!(dstData = AfxOpenRasterRegion(ras, &rgn, afxRasterAccess_W, &rgnSiz, &dstRowSiz))) AfxThrowError();
    else
    {
        afxNat dstRowCnt = rgn.whd[1];
        afxNat srcRowSiz = op->bufRowSiz;
        afxNat srcRowCnt = op->bufRowCnt;

        AfxAssert(dstRowSiz >= srcRowSiz);
        AfxAssert(dstRowCnt >= srcRowCnt);

        afxNat rowSiz = srcRowSiz ? srcRowSiz : dstRowSiz;
        afxNat rowCnt = srcRowCnt ? srcRowCnt : dstRowCnt;
        afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
        AfxAssert(rgnSiz >= dataSiz);

        if (AfxReadStreamAt(in, op->bufOffset, dataSiz, 0, dstData))
            AfxThrowError();

        AfxCloseRasterRegion(ras, &rgn);
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
    AfxAssertRange(ras->whd[0], rgn.offset[0], rgn.whd[0]);
    AfxAssertRange(ras->whd[1], rgn.offset[1], rgn.whd[1]);
    AfxAssertRange(ras->whd[2], rgn.offset[2], rgn.whd[2]);

    afxNat rgnSiz = 0;
    afxNat srcRowSiz = 0;
    void *srcData;

    if (!(srcData = AfxOpenRasterRegion(ras, &rgn, afxRasterAccess_R, &rgnSiz, &srcRowSiz))) AfxThrowError();
    else
    {
        afxNat srcRowCnt = rgn.whd[1];
        afxNat dstRowSiz = op->bufRowSiz;
        afxNat dstRowCnt = op->bufRowCnt;

        AfxAssert(srcRowSiz >= dstRowSiz);
        AfxAssert(srcRowCnt >= dstRowCnt);

        afxNat rowSiz = dstRowSiz ? dstRowSiz : srcRowSiz;
        afxNat rowCnt = dstRowCnt ? dstRowCnt : srcRowCnt;
        afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
        AfxAssert(rgnSiz >= dataSiz);

        if (AfxWriteStreamAt(out, op->bufOffset, dataSiz, 0, srcData))
            AfxThrowError();

        AfxCloseRasterRegion(ras, &rgn);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUpdateRaster(afxRaster ras, afxRasterIoOp const* op, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(op);
    AfxAssert(src);

    afxRasterRegion rgn = op->rgn;
    AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn.baseLayer, rgn.layerCnt);
    AfxAssertRange(ras->whd[0], rgn.offset[0], rgn.whd[0]);
    AfxAssertRange(ras->whd[1], rgn.offset[1], rgn.whd[1]);
    AfxAssertRange(ras->whd[2], rgn.offset[2], rgn.whd[2]);

    afxNat rgnSiz = 0;
    afxNat dstRowSiz = 0;
    void *dstData;

    if (!(dstData = AfxOpenRasterRegion(ras, &rgn, afxRasterAccess_RW, &rgnSiz, &dstRowSiz))) AfxThrowError();
    else
    {
        afxNat dstRowCnt = rgn.whd[1];
        afxNat srcRowSiz = op->bufRowSiz;
        afxNat srcRowCnt = op->bufRowCnt;

        AfxAssert(dstRowSiz >= srcRowSiz);
        AfxAssert(dstRowCnt >= srcRowCnt);

        afxNat rowSiz = srcRowSiz ? srcRowSiz : dstRowSiz;
        afxNat rowCnt = srcRowCnt ? srcRowCnt : dstRowCnt;
        afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
        AfxAssert(rgnSiz >= dataSiz);

        afxByte const* src2 = src;
        AfxCopy2(rowCnt * rgn.whd[2], rowSiz, &src2[op->bufOffset], dstData);

        AfxCloseRasterRegion(ras, &rgn);
    }
    return err;
}

_AVX afxError AfxDumpRaster(afxRaster ras, afxRasterIoOp const* op, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(dst);
    AfxAssert(op);
    
    afxRasterRegion rgn = op->rgn;
    AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn.baseLayer, rgn.layerCnt);
    AfxAssertRange(ras->whd[0], rgn.offset[0], rgn.whd[0]);
    AfxAssertRange(ras->whd[1], rgn.offset[1], rgn.whd[1]);
    AfxAssertRange(ras->whd[2], rgn.offset[2], rgn.whd[2]);

    afxNat rgnSiz = 0;
    afxNat srcRowSiz = 0;
    void *srcData;

    if (!(srcData = AfxOpenRasterRegion(ras, &rgn, afxRasterAccess_R, &rgnSiz, &srcRowSiz))) AfxThrowError();
    else
    {
        afxNat srcRowCnt = rgn.whd[1];
        afxNat dstRowSiz = op->bufRowSiz;
        afxNat dstRowCnt = op->bufRowCnt;

        AfxAssert(srcRowSiz >= dstRowSiz);
        AfxAssert(srcRowCnt >= dstRowCnt);

        afxNat rowSiz = dstRowSiz ? dstRowSiz : srcRowSiz;
        afxNat rowCnt = dstRowCnt ? dstRowCnt : srcRowCnt;
        afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
        AfxAssert(rgnSiz >= dataSiz);

        afxByte* dst2 = dst;
        AfxCopy2(rowCnt * rgn.whd[2], rowSiz, srcData, &dst2[op->bufOffset]);

        AfxCloseRasterRegion(ras, &rgn);
    }
    return err;
}

_AVX void AfxCloseRasterRegion(afxRaster ras, afxRasterRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssertRange(ras->lodCnt, rgn->lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->whd[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->whd[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->whd[2]);

    if (ras->unmap(ras, rgn))
        AfxThrowError();
};

_AVX void* AfxOpenRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxRasterAccess flags, afxNat *rgnSiz, afxNat *rowSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssertRange(ras->lodCnt, rgn->lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->whd[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->whd[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->whd[2]);
    
    AfxAssert(rgnSiz);
    AfxAssert(rowSiz);

    afxNat rowSiz2;

    void *ptr = NIL;

    if (ras->map(ras, rgn, flags, rgnSiz, &rowSiz2, &ptr))
        AfxThrowError();

    if (rowSiz)
        *rowSiz = rowSiz2;

    return ptr;
}

_AVX afxError AfxBufferizeRaster(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);

    afxRasterRegion srcRgn;
    srcRgn.baseLayer = 0;
    srcRgn.layerCnt = ras->layerCnt;
    srcRgn.offset[0] = 0;
    srcRgn.offset[1] = 0;
    srcRgn.offset[2] = 0;
    AfxGetRasterExtent(ras, 0, srcRgn.whd);

    afxNat siz = 0;

    for (afxNat i = 0; i < ras->lodCnt; i++)
    {
        srcRgn.lodIdx = i;
        siz += AfxMeasureRasterRegion(ras, &srcRgn);
    }

    void *maps = NIL;

    if (siz && !(maps = AfxReallocate(ras->maps, AFX_ALIGN(siz, AFX_SIMD_ALIGN), 1, AFX_SIMD_ALIGN, AfxHere()))) AfxThrowError();
    else
        ras->maps = maps;

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

_AVX afxRaster AfxAssembleRaster(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxNat cnt, afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(uri);
    afxRaster ras;

    if (AfxAssembleRastersFromTarga(dctx, usage, flags, cnt, uri, &ras))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}

_AVX afxRaster AfxAssembleCubemapRasters(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxUri const uri[6])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(uri);
    afxRaster ras;

    if (!(ras = AfxAssembleRaster(dctx, usage, flags | afxRasterFlag_CUBEMAP, 6, uri)))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}
