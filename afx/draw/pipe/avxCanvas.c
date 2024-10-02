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
#define _AVX_CANVAS_C
#include "../../dev/AvxImplKit.h"

_AVX afxResult AfxTestCanvas(avxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->flags & bitmask;
}

_AVX void AfxCountDrawBufferSlots(avxCanvas canv, afxNat* slotCnt, afxNat* colorSlotCnt)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(slotCnt || colorSlotCnt);

    if (slotCnt)
        *slotCnt = canv->slotCnt;

    if (colorSlotCnt)
        *colorSlotCnt = canv->colorCnt;
}

_AVX void AfxGetCanvasExtent(avxCanvas canv, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(whd);
    whd[0] = canv->whd[0];
    whd[1] = canv->whd[1];
    whd[2] = canv->whd[2];
}

_AVX afxBool AfxGetDepthBufferSlots(avxCanvas canv, afxNat* dSlotIdx, afxNat* sSlotIdx)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(dSlotIdx || sSlotIdx);
    afxBool rslt = 0;

    if (dSlotIdx)
    {
        *dSlotIdx = canv->dsSlotIdx[0];

        if (canv->dsSlotIdx[0] != AFX_INVALID_INDEX)
            ++rslt;
    }

    if (sSlotIdx)
    {
        *sSlotIdx = canv->dsSlotIdx[1];

        if (canv->dsSlotIdx[1] != AFX_INVALID_INDEX)
            ++rslt;
    }
    return rslt;
}

_AVX afxBool AfxGetDrawBuffers(avxCanvas canv, afxNat baseSlotIdx, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->slotCnt, baseSlotIdx, cnt);
    AfxAssert(rasters);
    afxBool rslt = 0;
    afxRaster ras;
    
    baseSlotIdx = AfxMin(baseSlotIdx, canv->slotCnt - 1);
    cnt = AfxMax(cnt, canv->slotCnt - baseSlotIdx);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat slotIdx = baseSlotIdx + i;
        
        if ((ras = canv->slots[slotIdx].ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
            ++rslt;
        }
        rasters[i] = ras;
    }
    return rslt;
}

_AVX afxBool AfxGetColorBuffers(avxCanvas canv, afxNat baseSlotIdx, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->colorCnt, baseSlotIdx, cnt);

    baseSlotIdx = AfxMin(baseSlotIdx, canv->colorCnt - 1);
    cnt = AfxMax(cnt, canv->colorCnt - baseSlotIdx);

    afxBool rslt = AfxGetDrawBuffers(canv, baseSlotIdx, cnt, rasters);
    AfxAssertObjects(rslt, rasters, afxFcc_RAS);
    return rslt;
}

_AVX afxBool AfxGetDepthBuffers(avxCanvas canv, afxRaster* depth, afxRaster* stencil)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(depth || stencil);
    afxBool rslt = 0;
    afxRaster ras = NIL;
    afxNat dSlotIdx = AFX_INVALID_HANDLE, sSlotIdx = AFX_INVALID_HANDLE;
    
    if (AfxGetDepthBufferSlots(canv, &dSlotIdx, &sSlotIdx))
    {
        if (depth)
        {
            if (AfxGetDrawBuffers(canv, dSlotIdx, 1, &ras))
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);
                *depth = ras;
                ++rslt;
            }
            else *depth = NIL;
        }

        if (stencil)
        {
            if (AfxGetDrawBuffers(canv, sSlotIdx, 1, &ras))
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);
                *stencil = ras;
                ++rslt;
            }
            else *stencil = NIL;
        }
    }
    return rslt;
}

_AVX afxError _AvxRelinkDrawBuffersCb(avxCanvas canv, afxBool managed, afxNat baseSlot, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->slotCnt, baseSlot, cnt);
    AfxAssert(rasters);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat slotIdx = baseSlot + i;
        afxRaster curr = canv->slots[slotIdx].ras;
        afxRaster ras = rasters ? rasters[i] : NIL;

        if (ras != curr)
        {
            if (ras)
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);

                if (!AfxTestRasterUsage(ras, afxRasterUsage_DRAW)) AfxThrowError();
                else
                {
                    if (canv->slots[slotIdx].fmt != AfxGetRasterFormat(ras)) AfxThrowError();
                    else
                    {
                        afxWhd ext;
                        AfxGetRasterExtent(ras, 0, ext);

                        if ((ext[0] < canv->whd[0]) && (ext[1] < canv->whd[1]) && (ext[2] < canv->whd[2])) AfxThrowError();
                        else
                        {
                            AfxReacquireObjects(1, &ras);
                            canv->slots[slotIdx].managed = !!managed;

                            if (!curr)
                                ++canv->linkedCnt;
                        }
                    }
                }

                if (err)
                    ras = NIL;
            }
            
            canv->slots[slotIdx].ras = ras;

            if (!ras)
                canv->slots[slotIdx].managed = TRUE;

            if (curr)
            {
                AfxAssertObjects(1, &curr, afxFcc_RAS);
                AfxReleaseObjects(1, &curr);

                if (!ras)
                    --canv->linkedCnt;
            }
        }
    }
    return err;
}

_AVX afxError AfxRelinkDrawBuffers(avxCanvas canv, afxNat baseSlot, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxTryAssertObjects(cnt, rasters, afxFcc_RAS);
    AfxAssertRange(canv->slotCnt, baseSlot, cnt);
    AfxAssert(rasters);

    if (!canv->relink)
    {
        if (_AvxRelinkDrawBuffersCb(canv, FALSE, baseSlot, cnt, rasters))
            AfxThrowError();
    }
    else if (canv->relink(canv, FALSE, baseSlot, cnt, rasters))
        AfxThrowError();

    return err;
}

_AVX afxError AfxRelinkDepthBuffer(avxCanvas canv, afxRaster depth)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxTryAssertObjects(1, &depth, afxFcc_RAS);
    afxNat slotIdx;

    if (!AfxGetDepthBufferSlots(canv, &slotIdx, NIL)) AfxThrowError();
    else
    {
        if (AfxRelinkDrawBuffers(canv, slotIdx, 1, &depth))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxRelinkStencilBuffer(avxCanvas canv, afxRaster stencil)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxTryAssertObjects(1, &stencil, afxFcc_RAS);
    afxNat slotIdx;

    if (!AfxGetDepthBufferSlots(canv, NIL, &slotIdx)) AfxThrowError();
    else
    {
        if (AfxRelinkDrawBuffers(canv, slotIdx, 1, &stencil))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxReadjustCanvas(avxCanvas canv, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(whd);

    for (afxNat i = 0; i < canv->slotCnt; i++)
    {
        afxRasterSlot* surf = &canv->slots[i];

        if (!surf->managed)
        {
            afxWhd rasWhd;
            AfxAssert(surf->ras);
            AfxGetRasterExtent(surf->ras, 0, rasWhd);

            if ((whd[0] > rasWhd[0]) ||
                (whd[1] > rasWhd[1]) ||
                (whd[2] > rasWhd[2])
                )
            {
                AfxThrowError();
            }
        }
    }
    AfxThrowError(); // incomplete
    return err;
}

_AVX afxError AfxPrintDrawBuffer(avxCanvas canv, afxNat slotIdx, afxNat portIdx, afxRasterIo const* op, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->slotCnt, slotIdx, 1);
    afxRaster ras;

    if (!AfxGetDrawBuffers(canv, slotIdx, 1, &ras)) AfxThrowError();
    else
    {
        if (op)
        {
            AfxAssertRange(canv->whd[0], op->rgn.origin[0], 1);
            AfxAssertRange(canv->whd[1], op->rgn.origin[1], 1);
            AfxAssertRange(canv->whd[2], op->rgn.origin[2], 1);
            AfxAssert2(canv->whd[0] >= op->rgn.whd[0], op->rgn.whd[0]);
            AfxAssert2(canv->whd[1] >= op->rgn.whd[1], op->rgn.whd[1]);
            AfxAssert2(canv->whd[2] >= op->rgn.whd[2], op->rgn.whd[2]);
            
            if (AfxPrintRaster(ras, op, 1, uri, portIdx))
                AfxThrowError();
        }
        else
        {
            afxRasterIo op2 = { 0 };
            AfxGetCanvasExtent(canv, op2.rgn.whd);

            if (AfxPrintRaster(ras, &op2, 1, uri, portIdx))
                AfxThrowError();
        }
    }
    return err;
}

_AVX afxError AfxRedoDrawBuffers(avxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);

    afxDrawContext dctx = AfxGetProvider(canv);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxRasterInfo texi = { 0 };
    texi.lodCnt = 1;
    texi.whd[0] = canv->whd[0];
    texi.whd[1] = canv->whd[1];
    texi.whd[2] = canv->whd[2];

    for (afxNat i = 0; i < canv->slotCnt; i++)
    {
        afxRasterSlot* surf = &canv->slots[i];

        if (surf->managed)
        {
            if (surf->ras)
                AfxRelinkDrawBuffers(canv, i, 1, NIL);

            texi.fmt = surf->fmt;
            texi.lodCnt = surf->sampleCnt;
            texi.usage = surf->usage | afxRasterUsage_DRAW;
            afxRaster ras;

            if (AfxAcquireRasters(dctx, 1, &texi, &ras)) AfxThrowError();
            else
            {
                if (!canv->relink)
                {
                    if (_AvxRelinkDrawBuffersCb(canv, TRUE, i, 1, &ras))
                        AfxThrowError();
                }
                else if (canv->relink(canv, TRUE, i, 1, &ras))
                    AfxThrowError();

                AfxReleaseObjects(1, &ras);
            }
        }
    }
    return err;
}

_AVX afxError _AvxCanvStdDtor(avxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    /// canv must be a valid avxCanvas handle.
    AfxAssertObjects(1, &canv, afxFcc_CANV);

    for (afxNat i = 0; i < canv->slotCnt; i++)
    {
        afxRaster ras = canv->slots[i].ras;

        if (ras)
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
            AfxReleaseObjects(1, &ras);
            canv->slots[i].ras = NIL;
        }
    }

    afxObjectStash const stashs[] =
    {
        {
            .cnt = canv->slotCnt,
            .siz = sizeof(canv->slots[0]),
            .var = (void**)&canv->slots
        }
    };
    AfxDeallocateInstanceData(canv, AFX_COUNTOF(stashs), stashs);

    return err;
}

_AVX afxError _AvxCanvStdCtor(avxCanvas canv, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = args[0];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat const* whd = args[1];
    afxNat surCnt = *(afxNat const *)args[2];
    afxSurfaceConfig const* surCfgs = args[3];

    if (!surCnt) AfxThrowError();
    else
    {
        canv->whd[0] = AfxMax(1, whd[0]);
        canv->whd[1] = AfxMax(1, whd[1]);
        canv->whd[2] = AfxMax(1, whd[2]);

        canv->readjust = NIL;// _SglReadjustCanvasCb;

        canv->flags = NIL;

        afxBool combinedDs = FALSE;
        afxNat depthInIdx = AFX_INVALID_INDEX;
        afxNat stencilInIdx = AFX_INVALID_INDEX;
        afxNat slotCnt = 0;
        afxNat colorCnt = 0;

        for (afxNat i = 0; i < surCnt; i++)
        {
            afxSurfaceConfig const* sur = &surCfgs[i];
            AfxAssert(sur->fmt);
            AfxAssert(sur->sampleCnt);

            if (AfxIsPixelFormatDepth(sur->fmt))
            {
                if (depthInIdx == AFX_INVALID_INDEX)
                    ++slotCnt;

                depthInIdx = i;

                if ((combinedDs = AfxIsPixelFormatCombinedDepthStencil(sur->fmt)))
                    stencilInIdx = depthInIdx;

                continue;
            }

            if (AfxIsPixelFormatStencil(sur->fmt))
            {
                if (stencilInIdx == AFX_INVALID_INDEX)
                    ++slotCnt;

                stencilInIdx = i;
                continue;
            }

            ++colorCnt;
            ++slotCnt;
        }

        canv->linkedCnt = 0;
        canv->colorCnt = colorCnt;
        canv->ownershipMask = NIL;

        afxObjectStash const stashs[] =
        {
            {
                .cnt = slotCnt,
                .siz = sizeof(canv->slots[0]),
                .var = (void**)&canv->slots
            }
        };

        if (AfxAllocateInstanceData(canv, AFX_COUNTOF(stashs), stashs)) AfxThrowError();
        else
        {
            AfxAssert(canv->slots);
            afxRasterSlot* surf;

            for (afxNat i = 0; i < slotCnt; i++)
            {
                surf = &canv->slots[i];
                surf->ras = NIL;
                surf->resolve = NIL;
                surf->managed = TRUE;
                surf->fmt = surCfgs[i].fmt;
                surf->usage = surCfgs[i].rasUsage | afxRasterUsage_DRAW;
                surf->flags = surCfgs[i].rasFlags;
                surf->sampleCnt = surCfgs[i].sampleCnt;
            }

            canv->slotCnt = slotCnt;

            if (stencilInIdx == AFX_INVALID_INDEX) canv->dsSlotIdx[1] = AFX_INVALID_INDEX;
            else
            {
                canv->dsSlotIdx[1] = (slotCnt - 1);

                surf = &canv->slots[canv->dsSlotIdx[1]];
                surf->ras = NIL;
                surf->resolve = NIL;
                surf->managed = TRUE;
                surf->fmt = surCfgs[stencilInIdx].fmt;
                surf->usage = surCfgs[stencilInIdx].rasUsage | afxRasterUsage_DRAW;
                surf->flags = surCfgs[stencilInIdx].rasFlags;
                surf->sampleCnt = surCfgs[stencilInIdx].sampleCnt;
                canv->flags |= afxCanvasFlag_HAS_STENCIL;
            }

            if (depthInIdx == AFX_INVALID_INDEX) canv->dsSlotIdx[0] = AFX_INVALID_INDEX;
            else
            {
                if (combinedDs)
                {
                    canv->dsSlotIdx[0] = canv->dsSlotIdx[1];
                    canv->flags |= afxCanvasFlag_COMBINED_DS;
                }
                else if (stencilInIdx != AFX_INVALID_INDEX)
                    canv->dsSlotIdx[0] = (canv->dsSlotIdx[1] - 1);
                else
                    canv->dsSlotIdx[0] = (slotCnt - 1);

                surf = &canv->slots[canv->dsSlotIdx[0]];
                surf->ras = NIL;
                surf->resolve = NIL;
                surf->managed = TRUE;
                surf->fmt = surCfgs[depthInIdx].fmt;
                surf->usage = surCfgs[depthInIdx].rasUsage | afxRasterUsage_DRAW;
                surf->flags = surCfgs[depthInIdx].rasFlags;
                surf->sampleCnt = surCfgs[depthInIdx].sampleCnt;
                canv->flags |= afxCanvasFlag_HAS_DEPTH;
            }

            if (err)
            {
                AfxDeallocateInstanceData(canv, AFX_COUNTOF(stashs), stashs);
            }
        }
    }
    return err;
}

_AVX afxClassConfig const _AvxCanvStdImplementation =
{
    .fcc = afxFcc_CANV,
    .name = "Canvas",
    .desc = "Buffered Drawing Canvas",
    .fixedSiz = sizeof(AFX_OBJECT(avxCanvas)),
    .ctor = (void*)_AvxCanvStdCtor,
    .dtor = (void*)_AvxCanvStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxCoacquireCanvas(afxDrawContext dctx, afxWhd const whd, afxNat surCnt, afxSurfaceConfig const surCfg[], afxNat cnt, avxCanvas canvases[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(canvases);
    AfxAssert(surCnt);
    AfxAssert(surCfg);
    AfxAssert(whd);
    AfxAssert(whd[0]);
    AfxAssert(whd[1]);

    afxClass* cls = AfxGetCanvasClass(dctx);
    AfxAssertClass(cls, afxFcc_CANV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)canvases, (void const*[]) { dctx, whd, &surCnt, surCfg }))
        AfxThrowError();

    return err;
}
