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

_AVXINL afxNat AfxGetRasterBpp(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return AfxGetBpp(ras->fmt);
}

_AVXINL afxNat AfxCountRasterLods(afxRaster ras)
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

_AVXINL void AfxGetRasterSwizzling(afxRaster ras, afxNat subIdx, afxColorSwizzling* csw)
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
}

_AVXINL void AfxDescribeRaster(afxRaster ras, afxRasterInfo* desc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(desc);
    desc->lodCnt = ras->lodCnt;
    desc->sampleCnt = ras->sampleCnt;
    desc->whd[0] = ras->whd[0];
    desc->whd[1] = ras->whd[1];
    desc->whd[2] = ras->whd[2];
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

    afxWhd whd = { ras->whd[0], ras->whd[1], ras->whd[2] };
    while (lodIdx--)
    {
        whd[0] = whd[0] >> 1;
        whd[1] = whd[1] >> 1;

        if (is3d)
            whd[2] = whd[2] >> 1;
    }
    AfxAssert(whd[0]);
    AfxAssert(whd[1]);
    AfxAssert(whd[2]);

    afxNat pixelStride = AFX_ALIGN(AfxGetRasterBpp(ras), AFX_BYTE_SIZE) / AFX_BYTE_SIZE;
    afxNat rowStride = AFX_ALIGN(pixelStride * whd[0], AFX_SIMD_ALIGN);
    afxNat depthStride = rowStride * whd[1];
    AfxAssert(layout);
    layout->depthStride = depthStride;
    layout->rowStride = rowStride;
    layout->offset = 0;
    layout->siz = is3d ? whd[2] * depthStride : depthStride;
    return !err;
}

////////////////////////////////////////////////////////////////////////////////
// SERIALIZATION AND STORAGE                                                  //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUploadRaster(afxRaster ras, afxRasterIo const* op, afxNat lodCnt, afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(op);
    
    afxRasterRegion rgn = op->rgn;
    AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
    AfxAssertRange(ras->whd[0], rgn.origin[0], rgn.whd[0]);
    AfxAssertRange(ras->whd[1], rgn.origin[1], rgn.whd[1]);
    AfxAssertRange(ras->whd[2], rgn.origin[2], rgn.whd[2]);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat portIdx = 0;
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, portIdx);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxTransferRequest req = { 0 };
    req.img.ras = ras;
    req.img.op = *op;
    req.img.op.rgn.origin[0] = AfxMin(req.img.op.rgn.origin[0], ras->whd[0] - 1);
    req.img.op.rgn.origin[1] = AfxMin(req.img.op.rgn.origin[1], ras->whd[1] - 1);
    req.img.op.rgn.origin[2] = AfxMin(req.img.op.rgn.origin[2], ras->whd[2] - 1);
    req.img.op.rgn.whd[0] = AfxClamp(req.img.op.rgn.whd[0], 1, ras->whd[0]);
    req.img.op.rgn.whd[1] = AfxClamp(req.img.op.rgn.whd[1], 1, ras->whd[1]);
    req.img.op.rgn.whd[2] = AfxClamp(req.img.op.rgn.whd[2], 1, ras->whd[2]);
    
    req.img.iob = in;
    req.dstFcc = afxFcc_RAS;
    req.srcFcc = afxFcc_IOB;
    
    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxDownloadRaster(afxRaster ras, afxRasterIo const* op, afxNat lodCnt, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(op);

    afxRasterRegion rgn = op->rgn;
    AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
    AfxAssertRange(ras->whd[0], rgn.origin[0], rgn.whd[0]);
    AfxAssertRange(ras->whd[1], rgn.origin[1], rgn.whd[1]);
    AfxAssertRange(ras->whd[2], rgn.origin[2], rgn.whd[2]);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat portIdx = 0;
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, portIdx);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxTransferRequest req = { 0 };
    req.img.ras = ras;
    req.img.op = *op;
    req.img.op.rgn.origin[0] = AfxMin(req.img.op.rgn.origin[0], ras->whd[0] - 1);
    req.img.op.rgn.origin[1] = AfxMin(req.img.op.rgn.origin[1], ras->whd[1] - 1);
    req.img.op.rgn.origin[2] = AfxMin(req.img.op.rgn.origin[2], ras->whd[2] - 1);
    req.img.op.rgn.whd[0] = AfxClamp(req.img.op.rgn.whd[0], 1, ras->whd[0]);
    req.img.op.rgn.whd[1] = AfxClamp(req.img.op.rgn.whd[1], 1, ras->whd[1]);
    req.img.op.rgn.whd[2] = AfxClamp(req.img.op.rgn.whd[2], 1, ras->whd[2]);
    
    req.img.iob = out;
    req.srcFcc = afxFcc_RAS;
    req.dstFcc = afxFcc_IOB;
    
    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx))
            AfxThrowError();
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUpdateRaster(afxRaster ras, afxRasterIo const* op, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(op);
    AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
    AfxAssertRange(ras->whd[0], op->rgn.origin[0], op->rgn.whd[0]);
    AfxAssertRange(ras->whd[1], op->rgn.origin[1], op->rgn.whd[1]);
    AfxAssertRange(ras->whd[2], op->rgn.origin[2], op->rgn.whd[2]);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat portIdx = 0;
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, portIdx);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    
    afxNat unusedBpl;
    afxTransferRequest req = { 0 };
    req.img.ras = ras;
    req.img.op = *op;
    req.img.op.rgn.origin[0] = AfxMin(req.img.op.rgn.origin[0], ras->whd[0] - 1);
    req.img.op.rgn.origin[1] = AfxMin(req.img.op.rgn.origin[1], ras->whd[1] - 1);
    req.img.op.rgn.origin[2] = AfxMin(req.img.op.rgn.origin[2], ras->whd[2] - 1);
    req.img.op.rgn.whd[0] = AfxClamp(req.img.op.rgn.whd[0], 1, ras->whd[0]);
    req.img.op.rgn.whd[1] = AfxClamp(req.img.op.rgn.whd[1], 1, ras->whd[1]);
    req.img.op.rgn.whd[2] = AfxClamp(req.img.op.rgn.whd[2], 1, ras->whd[2]);
    
    if (!req.img.op.rowStride)
    {
        afxBool is3d = AfxRasterIs3d(ras);
        afxRasterLayout layout;
        AfxQueryRasterLayout(ras, req.img.op.rgn.lodIdx, (is3d ? 0 : req.img.op.rgn.whd[2]), &layout);
        req.img.op.rowStride = layout.rowStride;
        req.img.op.rowCnt = req.img.op.rgn.whd[1];
    }

    req.dstFcc = afxFcc_RAS;
    req.img.src = src;

    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxDumpRaster(afxRaster ras, afxRasterIo const* op, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(op);
    AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
    AfxAssertRange(ras->whd[0], op->rgn.origin[0], op->rgn.whd[0]);
    AfxAssertRange(ras->whd[1], op->rgn.origin[1], op->rgn.whd[1]);
    AfxAssertRange(ras->whd[2], op->rgn.origin[2], op->rgn.whd[2]);

    afxDrawContext dctx = AfxGetRasterContext(ras);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat portIdx = 0;
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, portIdx);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxNat unusedBpl;
    afxTransferRequest req = { 0 };
    req.img.ras = ras;
    req.img.op = *op;
    req.img.op.rgn.origin[0] = AfxMin(req.img.op.rgn.origin[0], ras->whd[0] - 1);
    req.img.op.rgn.origin[1] = AfxMin(req.img.op.rgn.origin[1], ras->whd[1] - 1);
    req.img.op.rgn.origin[2] = AfxMin(req.img.op.rgn.origin[2], ras->whd[2] - 1);
    req.img.op.rgn.whd[0] = AfxClamp(req.img.op.rgn.whd[0], 1, ras->whd[0]);
    req.img.op.rgn.whd[1] = AfxClamp(req.img.op.rgn.whd[1], 1, ras->whd[1]);
    req.img.op.rgn.whd[2] = AfxClamp(req.img.op.rgn.whd[2], 1, ras->whd[2]);

    req.srcFcc = afxFcc_RAS;
    req.img.dst = dst;

    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxRasStdDtor(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    return err;
}

_AVX afxError _AvxRasStdCtor(afxRaster ras, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxRasterInfo const *texi = ((afxRasterInfo const *)cookie->udd[1]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(ras);

    ras->subCnt = 0;
    ras->subs = NIL;

    //ras->swizzling = NIL; //&AFX_STD_COLOR_SWIZZLING;
    ras->sampleCnt = AfxMax(texi->sampleCnt, 1);

    ras->lodCnt = AfxMax(texi->lodCnt, 1);
    ras->whd[0] = AfxMax(texi->whd[0], 1);
    ras->whd[1] = AfxMax(texi->whd[1], 1);
    ras->whd[2] = AfxMax(texi->whd[2], 1);

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

        flags = (afxRasterFlag_3D | afxRasterFlag_CUBEMAP | afxRasterFlag_LAYERED);

        if ((ras->flags = (texi->flags & flags)) == flags) AfxThrowError();
        else
        {
            if (ras->whd[2] > 1)
                if (!(ras->flags & flags))
                    ras->flags = afxRasterFlag_LAYERED;
        }
    }
    return err;
}

_AVX afxClassConfig const _AvxRasStdImplementation =
{
    .fcc = afxFcc_RAS,
    .name = "Raster",
    .desc = "Formatted Video Buffer",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxRaster)),
    .ctor = (void*)_AvxRasStdCtor,
    .dtor = (void*)_AvxRasStdDtor
};

////////////////////////////////////////////////////////////////////////////////

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

    if (AfxAssembleRasters(dctx, usage, flags, dir, cnt, layers, &ras))
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
