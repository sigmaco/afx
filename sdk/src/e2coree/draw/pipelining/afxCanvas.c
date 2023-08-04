/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_CANVAS_C
#define _AFX_DRAW_CONTEXT_C
#include "../_classified/afxDrawClassified.h"

_AFXINL void AfxBeginCanvasBlueprint(afxCanvasBlueprint *blueprint, afxDrawContext dctx, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(extent);
    *blueprint = (afxCanvasBlueprint) { 0 };
    blueprint->dctx = dctx;
    AfxCanvasBlueprintSetExtent(blueprint, extent);
    AfxCanvasBlueprintSetDepth(blueprint, NIL, NIL);
}

_AFXINL void AfxCanvasBlueprintSetExtent(afxCanvasBlueprint *blueprint, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(extent);
    blueprint->extent[0] = AfxMax(extent[0], 1);
    blueprint->extent[1] = AfxMax(extent[1], 1);
    blueprint->extent[2] = AfxMax(extent[2], 1);
}

_AFXINL void AfxCanvasBlueprintSetDepth(afxCanvasBlueprint *blueprint, afxPixelFormat depth, afxPixelFormat stencil)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(!depth || !(AFX_PFD_S8 > depth && AFX_PFD_D32FS8 < depth));
    AfxAssert(!stencil || !(AFX_PFD_S8 > stencil && AFX_PFD_D32FS8 < stencil));
    blueprint->ds[0].fmt = depth;
    blueprint->ds[1].fmt = stencil;
}

_AFXINL void AfxCanvasBlueprintAddRaster(afxCanvasBlueprint *blueprint, afxPixelFormat fmt, afxTextureFlags usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(fmt);
    blueprint->rasters[blueprint->rasterCnt].surf = NIL;
    blueprint->rasters[blueprint->rasterCnt].fmt = fmt;
    blueprint->rasters[blueprint->rasterCnt].usage = usage;
    ++blueprint->rasterCnt;
}

_AFXINL void AfxCanvasBlueprintAddExistingRaster(afxCanvasBlueprint *blueprint, afxSurface existing)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(existing);
    blueprint->rasters[blueprint->rasterCnt].surf = existing;
    blueprint->rasters[blueprint->rasterCnt].fmt = NIL;
    blueprint->rasters[blueprint->rasterCnt].usage = NIL;
    ++blueprint->rasterCnt;
}

_AFX afxError _AfxCanvFreeAllSurfaces(afxCanvas canv)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);

    if (canv->annexes)
    {
        for (afxNat i = 0; i < canv->annexCnt; i++)
        {
            afxSurface surf = canv->annexes[i].surf;

            if (surf)
            {
                AfxTryAssertObject(surf, AFX_FCC_SURF);
                AfxReleaseObject(&surf->tex.obj);
                canv->annexes[i].surf = NIL;
            }
        }
    }
    return err;
}

_AFX void AfxFlagCanvas(afxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    canv->flags |= bitmask;
}

_AFX void AfxUnflagCanvas(afxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    canv->flags &= ~bitmask;
}

_AFX afxResult AfxTestCanvas(afxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    return canv->flags & bitmask;
}

_AFX afxBool AfxGetAnnexedStencilSurface(afxCanvas canv, afxSurface *stencil)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxSurface surf = canv->dsIdx[1] != AFX_INVALID_INDEX ? canv->annexes[canv->dsIdx[1]].surf : NIL;

    if (surf)
    {
        AfxAssertObject(surf, AFX_FCC_SURF);

        if (stencil)
            *stencil = surf;
    }
    return !!surf;
}

_AFX afxBool AfxGetAnnexedDepthSurface(afxCanvas canv, afxSurface *depth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxSurface surf = canv->dsIdx[0] != AFX_INVALID_INDEX ? canv->annexes[canv->dsIdx[0]].surf : NIL;

    if (surf)
    {
        AfxAssertObject(surf, AFX_FCC_SURF);

        if (depth)
            *depth;
    }
    return !!surf;
}

_AFX afxBool AfxGetAnnexedRasterSurface(afxCanvas canv, afxNat rasIdx, afxSurface *raster)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    AfxAssert(rasIdx < canv->rasterCnt);

    afxBool rslt;
    afxSurface surf2;
    if ((rslt = !!(surf2 = canv->annexes[rasIdx].surf)))
    {
        AfxAssertObject(surf2, AFX_FCC_SURF);

        if (raster)
            *raster = surf2;
    }
    return rslt;
}

_AFX afxNat AfxGetAnnexedRasterSurfaceCount(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    return canv->rasterCnt;
}

_AFX afxBool AfxGetAnnexedSurface(afxCanvas canv, afxNat surfIdx, afxSurface *surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    AfxAssert(surfIdx < canv->annexCnt);
    
    afxBool rslt;
    afxSurface surf2;
    if ((rslt = !!(surf2 = canv->annexes[surfIdx].surf)))
    {
        AfxAssertObject(surf2, AFX_FCC_SURF);

        if (surf)
            *surf = surf2;
    }
    return rslt;
}

_AFX afxNat AfxGetAnnexedSurfaceCount(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    return canv->annexCnt;
}

_AFX afxError AfxReadjustCanvas(afxCanvas canv, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
	AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);

    if (0 == (extent[0] * extent[1] * extent[2])) AfxThrowError();
    else
    {
        afxWhd cur = { canv->whd[0], canv->whd[1], canv->whd[2] };
        afxWhd whd;

        afxSurface surf;

        for (afxNat i = 0; i < canv->annexCnt; i++)
        {
            surf = canv->annexes[i].surf;

            if (surf)
            {
                AfxAssertObject(surf, AFX_FCC_SURF);

                AfxGetTextureExtent(&surf->tex, 0, whd);

                //if (extent[0] > whd[0] || extent[1] > whd[1] || extent[2] > whd[2])
                    //if (canv->ownershipMask & AFX_FLAG(i)) // only owned here ones will be resized. Should works fine with dout due to implementation hide works to resize its buffers.
                        //if (AfxReadjustTexture(&surf->tex, canv->whd))
                            //AfxThrowError();
            }

            if (err)
                break;
        }

        if (!err)
        {
            if (canv->vmt->ext && canv->vmt->ext(canv, cur, extent)) AfxThrowError();
            else
            {
                canv->whd[0] = extent[0];
                canv->whd[1] = extent[1];
                canv->whd[2] = extent[2];

                afxEvent ev;
                AfxEventDeploy(&ev, AFX_EVENT_CANV_RESIZ, &canv->obj, canv->whd);
                AfxObjectEmitEvent(&canv->obj, &ev);

                AfxFlagCanvas(canv, AFX_FLAG(AFX_CANV_FLAG_REVALIDATE));
            }
        }
    }
    return err;
}

_AFX void AfxGetCanvasExtent(afxCanvas canv, afxWhd extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    AfxAssert(extent);
    extent[0] = canv->whd[0];
    extent[1] = canv->whd[1];
    extent[2] = canv->whd[2];
}

_AFX void AfxGetCanvasContext(afxCanvas canv, afxDrawContext *dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxDrawContext dctx2 = AfxObjectGetProvider(&canv->obj);
    AfxAssertObject(dctx2, AFX_FCC_DCTX);
    *dctx = dctx2;
}

_AFX afxError AfxBuildCanvases(afxCanvasBlueprint const *blueprint, afxNat cnt, afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(blueprint->dctx, AFX_FCC_DCTX);
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(canv);
    AfxEntry("dctx=%p,cnt=%u,blueprint=%p,canv=%p", blueprint->dctx, cnt, blueprint, canv);

    for (afxNat i = 0; i < cnt; i++)
    {
        if (AfxClassAcquireObjects(AfxGetCanvasClass(blueprint->dctx), NIL, 1, blueprint, (afxObject**)&canv[i], AfxSpawnHint())) AfxThrowError();
        else
        {
            AfxAssertObject(canv[i], AFX_FCC_CANV);
        }
    }
    return err;
};

_AFX afxBool _AfxCanvEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxCanvas canv = (void*)obj;
    AfxAssertObject(canv, AFX_FCC_CANV);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxCanvEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxCanvas canv = (void*)obj;
    AfxAssertObject(canv, AFX_FCC_CANV);
    (void)watched;
    (void)ev;

    switch (ev->type)
    {
    case AFX_EVENT_OBJ_DESTROYED:
    {
        for (afxNat i = 0; i < canv->annexCnt; i++)
        {
            afxSurface surf;
            AfxGetAnnexedSurface(canv, i, &surf);

            if (&surf->tex.obj == watched)
            {
                canv->annexes[i].surf = NIL;
                AfxThrowError();
            }
        }
        break;
    }
#if 0
    case AFX_EVENT_TEX_RESIZ:
    {
        afxTexture tex = (void*)watched;
        AfxAssertObject(tex, AFX_FCC_TEX);
        afxWhd extent;
        AfxGetTextureExtent(tex, 0, extent);        
        AfxReadjustCanvas(canv, extent);
        break;
    }
#endif
    case AFX_EVENT_DOUT_EXTENT:
    {
        afxDrawOutput dout = (void*)watched;
        AfxAssertObject(dout, AFX_FCC_DOUT);
        afxWhd extent;
        AfxGetDrawOutputExtent(dout, extent);
        AfxReadjustCanvas(canv, extent);
        break;
    }
    case AFX_EVENT_DOUT_REBUF:
    {
        afxNat idx = 0;

        if (canv->udd[0] == watched && canv->udd[1] == ev->udd[0])
        {
            afxDrawOutput dout = (void*)watched;
            AfxAssertObject(dout, AFX_FCC_DOUT);
            afxSurface surf;

            if (AfxGetAnnexedSurface(canv, idx, &surf))
            {
                AfxAssertObject(surf, AFX_FCC_SURF);
                canv->annexes[idx].surf = NIL;
                AfxObjectRemoveEventFilter(&surf->tex.obj, &canv->obj);
                AfxReleaseObject(&surf->tex.obj);
            }

            if (AfxGetDrawOutputBuffer(dout, idx, &surf))
            {
                AfxAssertObject(surf, AFX_FCC_SURF);
                canv->annexes[idx].surf = surf;
                AfxObjectInstallEventFilter(&surf->tex.obj, &canv->obj);
                AfxObjectReacquire(&surf->tex.obj, NIL, NIL, NIL, NIL);
            }
        }
        break;
    }
    default:break;
    }
    return FALSE;
}

_AFX afxError _AfxCanvDtor(afxCanvas canv)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(canv, AFX_FCC_CANV);

    afxDrawContext dctx = AfxObjectGetProvider(&canv->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    _AfxCanvFreeAllSurfaces(canv);

    if (canv->vmt->dtor && canv->vmt->dtor(canv))
        AfxThrowError();

    AfxAssert(!canv->idd);

    AfxDeallocate(mem, canv->annexes);

    return err;
}

_AFX afxError _AfxCanvCtor(void *cache, afxNat idx, afxCanvas canv, afxCanvasBlueprint const *blueprints)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    (void)cache;
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxDrawContext dctx = AfxObjectGetProvider(&canv->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);
    afxCanvasBlueprint const *blueprint = &blueprints[idx];

    canv->udd[0] = blueprint->udd[0];
    canv->udd[1] = blueprint->udd[1];

    canv->whd[0] = blueprint->extent[0];
    canv->whd[1] = blueprint->extent[1];
    canv->whd[2] = blueprint->extent[2];

    afxWhd whd;
    afxSurface surf;
    afxPixelFormat fmt;

    if (0 == (canv->whd[0] * canv->whd[1] * canv->whd[2]))
    {
        for (afxNat i = 0; i < blueprint->rasterCnt; i++)
        {
            if ((surf = blueprint->rasters[i].surf))
            {
                AfxAssertObject(surf, AFX_FCC_SURF);

                AfxGetTextureExtent(&surf->tex, 0, whd);

                canv->whd[0] = AfxMin(canv->whd[0], whd[0]);
                canv->whd[1] = AfxMin(canv->whd[1], whd[1]);
                canv->whd[2] = AfxMin(canv->whd[2], whd[2]);
            }
        }

        for (afxNat i = 0; i < sizeof(blueprint->ds) / sizeof(blueprint->ds[0]); i++)
        {
            if ((surf = blueprint->ds[i].surf))
            {
                AfxAssertObject(surf, AFX_FCC_SURF);

                AfxGetTextureExtent(&surf->tex, 0, whd);

                canv->whd[0] = AfxMin(canv->whd[0], whd[0]);
                canv->whd[1] = AfxMin(canv->whd[1], whd[1]);
                canv->whd[2] = AfxMin(canv->whd[2], whd[2]);
            }
        }
    }

    if (0 == (canv->whd[0] * canv->whd[1] * canv->whd[2])) AfxThrowError();
    else
    {
        canv->dsIdx[0] = AFX_INVALID_INDEX;
        canv->dsIdx[1] = AFX_INVALID_INDEX;

        afxPixelFormat depthFmt = blueprint->ds[0].fmt;
        afxPixelFormat stencilFmt = blueprint->ds[1].fmt;
        afxBool combinedDs;

#ifndef _AFX_OPTIMIZATION_CANV_DS_FMT_ROBUSTNESS

        if (depthFmt == AFX_PFD_D24S8 && (!stencilFmt))
            depthFmt = AFX_PFD_D24;

        if ((!depthFmt) && stencilFmt == AFX_PFD_D24S8)
            depthFmt = AFX_PFD_S8;

#ifndef _AFX_OPTIMIZATION_CANV_DS_FMT_COMBO

        if ((depthFmt == AFX_PFD_D24 && stencilFmt == AFX_PFD_S8))
            depthFmt = (stencilFmt = AFX_PFD_D24S8);
#endif
#endif
        combinedDs = (depthFmt && stencilFmt && depthFmt == stencilFmt);

        canv->annexCnt = blueprint->rasterCnt;

        if (depthFmt || stencilFmt)
        {
            if (combinedDs) ++canv->annexCnt;
            else
            {
                if (depthFmt)
                    ++canv->annexCnt;

                if (stencilFmt)
                    ++canv->annexCnt;
            }
        }

        canv->rasterCnt = 0;
        canv->ownershipMask = NIL;

        if (!(canv->annexes = AfxAllocate(mem, canv->annexCnt * sizeof(canv->annexes[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < canv->annexCnt; i++)
                AfxZero(&canv->annexes[i], sizeof(canv->annexes[0]));

            for (afxNat i = 0; i < blueprint->rasterCnt; i++)
            {
                if ((surf = blueprint->rasters[i].surf))
                {
                    if (!AfxTestTexture(&surf->tex, AFX_TEX_USAGE_DRAW)) AfxThrowError();
                    else
                    {
                        AfxGetTextureExtent(&surf->tex, 0, whd);

                        if (canv->whd[0] > whd[0] || canv->whd[1] > whd[1] || canv->whd[2] > whd[2]) AfxThrowError();
                        else
                        {
                            if (!(AfxObjectReacquire(&(surf->tex.obj), &canv->obj, NIL, NIL, NIL))) AfxThrowError();
                            else
                            {
                                canv->annexes[i].surf = surf;
                                canv->annexes[i].usage = AfxTestTexture(&surf->tex, AFX_N32_MAX);
                                canv->annexes[i].fmt = AfxGetTextureFormat(&surf->tex);
                                ++canv->rasterCnt;
                                //canv->ownershipMask |= AFX_FLAG(i);
                            }
                        }
                    }
                }
                else
                {
                    fmt = blueprint->rasters[i].fmt;

                    if (!fmt) AfxThrowError();
                    else
                    {
                        AfxAssert(!(blueprint->rasters[i].usage & AFX_TEX_USAGE_DRAW));
                        afxTextureFlags usage = blueprint->rasters[i].usage & ~(AFX_TEX_USAGE_DRAW);
                        usage |= AFX_TEX_USAGE_DRAW;

                        if (!(surf = AfxAcquireSurface(dctx, fmt, canv->whd, usage))) AfxThrowError();
                        else
                        {
                            AfxAssertObject(surf, AFX_FCC_SURF);
                            AfxAssert(AfxTestTexture(&surf->tex, AFX_TEX_USAGE_DRAW));
                            canv->annexes[i].surf = surf;
                            canv->annexes[i].usage = usage;
                            canv->annexes[i].fmt = fmt;
                            canv->ownershipMask |= AFX_FLAG(i);
                            ++canv->rasterCnt;
                        }
                    }
                }
            }

            if (!err)
            {
                for (afxNat i = 0; i < sizeof(blueprint->ds) / sizeof(blueprint->ds[0]); i++)
                {
                    if ((surf = blueprint->ds[i].surf))
                    {
                        if (!AfxTestTexture(&surf->tex, AFX_TEX_USAGE_DRAW)) AfxThrowError();
                        else
                        {
                            AfxGetTextureExtent(&surf->tex, 0, whd);

                            if (canv->whd[0] > whd[0] || canv->whd[1] > whd[1] || canv->whd[2] > whd[2]) AfxThrowError();
                            else
                            {
                                if (!(AfxObjectReacquire(&(surf->tex.obj), &canv->obj, NIL, NIL, NIL))) AfxThrowError();
                                else
                                {
                                    canv->dsIdx[i] = canv->rasterCnt + i;
                                    canv->annexes[canv->dsIdx[i]].surf = surf;
                                    canv->annexes[canv->dsIdx[i]].usage = AfxTestTexture(&surf->tex, AFX_N32_MAX);
                                    canv->annexes[canv->dsIdx[i]].fmt = AfxGetTextureFormat(&surf->tex);

                                    //canv->ownershipMask |= AFX_FLAG(canv->rasterCnt + i);
                                }
                            }
                        }
                    }
                    else
                    {
                        fmt = blueprint->ds[i].fmt;

                        if (fmt)
                        {
                            AfxAssert(!(blueprint->ds[i].usage & AFX_TEX_USAGE_DRAW));
                            afxTextureFlags usage = blueprint->ds[i].usage & ~(AFX_TEX_USAGE_DRAW);
                            usage |= AFX_TEX_USAGE_DRAW;

                            if (!(surf = AfxAcquireSurface(dctx, fmt, canv->whd, usage))) AfxThrowError();
                            else
                            {
                                AfxAssertObject(surf, AFX_FCC_SURF);
                                AfxAssert(AfxTestTexture(&surf->tex, AFX_TEX_USAGE_DRAW));

                                canv->dsIdx[i] = canv->rasterCnt + i;
                                canv->annexes[canv->dsIdx[i]].surf = surf;
                                canv->annexes[canv->dsIdx[i]].usage = usage;
                                canv->annexes[canv->dsIdx[i]].fmt = fmt;

                                canv->ownershipMask |= AFX_FLAG(canv->dsIdx[i]);
                            }
                        }
                    }
                }

                if (!err)
                {
                    canv->vmt = NIL;
                    canv->idd = NIL;

                    if (dctx->vmt->canv && dctx->vmt->canv(canv)) AfxThrowError();
                    else
                    {
                        AfxAssert(canv->vmt);

                        for (afxNat i = 0; i < canv->annexCnt; i++)
                        {
                            AfxObjectInstallEventFilter(&canv->annexes[i].surf->tex.obj, &canv->obj);
                        }
                    }
                }
            }

            if (err)
            {
                _AfxCanvFreeAllSurfaces(canv);

                AfxDeallocate(mem, canv->annexes);
            }
        }
    }
    return err;
}

_AFX afxClassSpecification const _AfxCanvClassSpec;

afxClassSpecification const _AfxCanvClassSpec =
{
    AFX_FCC_CANV,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxCanvas)),
    NIL,
    (void*)_AfxCanvCtor,
    (void*)_AfxCanvDtor,
    .event = _AfxCanvEventHandler,
    .eventFilter = _AfxCanvEventFilter,
    "afxCanvas",
    NIL
};
