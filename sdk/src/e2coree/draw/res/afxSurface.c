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

#define _AFX_SURFACE_C
#define _AFX_TEXTURE_C
#include "../_classified/afxDrawClassified.h"

_AFX afxSurfaceState AfxGetSurfaceState(afxSurface surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);
    return surf->state;
}

_AFX afxBool AfxSurfaceIsPresentable(afxSurface surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);
    return surf->state == AFX_SURF_STATE_PRESENTABLE;
}

_AFX afxDrawContext AfxGetSurfaceContext(afxSurface surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);
    afxDrawContext dctx = AfxObjectGetProvider(&surf->tex.obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    return dctx;
}

_AFX afxSurface AfxAcquireSurface(afxDrawContext dctx, afxPixelFormat fmt, afxWhd const extent, afxFlags usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    AfxAssert(usage & AFX_TEX_USAGE_DRAW);
    AfxEntry("fmt=%u,whd=[%u,%u,%u],usage=%u", fmt, extent[0], extent[1], extent[2], usage);
    afxSurface surf = NIL;

    usage &= AFX_TEX_USAGE_DRAW;

    if (fmt >= AFX_PFD_S8 && fmt <= AFX_PFD_D32FS8)
        usage |= AFX_TEX_USAGE_DRAW;
    else
        usage |= AFX_TEX_USAGE_DRAW;

    afxTextureBlueprint texb;
    AfxAcquireTextureBlueprint(&texb, dctx, extent, fmt, usage);
    AfxTextureBlueprintAddImage(&texb, fmt, extent, NIL, NIL);

    if (AfxClassAcquireObjects(AfxGetSurfaceClass(dctx), NIL, 1, &texb, (afxObject**)&surf, AfxSpawnHint()))
        AfxThrowError();

    AfxTextureBlueprintEnd(&texb, 0, NIL);

    AfxTryAssertObject(surf, AFX_FCC_SURF);

    return surf;
};

_AFX afxBool _AfxSurfEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxSurface surf = (void*)obj;
    AfxAssertObject(surf, AFX_FCC_SURF);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxSurfEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxSurface surf = (void*)obj;
    AfxAssertObject(surf, AFX_FCC_SURF);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxSurfDtor(afxSurface surf)
{
    AfxEntry("surf=%p", surf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);

    while (1)
    {
        if (!surf->swapchain.chain) break;
        else
        {
            AfxAssert(surf->state == AFX_SURF_STATE_PENDING);

            // AfxPopLinkage(&canv->queue); // we can't do it here. We need wait for draw context to liberate it.

            afxDrawOutput dout = AfxGetLinker(&surf->swapchain);
            AfxAssertObject(dout, AFX_FCC_DOUT);
            //_SglDoutProcess(dout); // process until draw output ends its works and unlock this canvas.
            //AfxYieldThreading();
        }
        break;
    }

    if (surf->vmt->dtor && surf->vmt->dtor(surf))
        AfxThrowError();

    AfxAssert(!surf->idd);

    return err;
}

_AFX afxError _AfxSurfCtor(void *cache, afxNat idx, afxSurface surf, void *args)
{
    (void)cache;
    (void)args;
    (void)idx;
    AfxEntry("surf=%p", surf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);

    afxDrawContext dctx = AfxGetSurfaceContext(surf);

    surf->state = AFX_SURF_STATE_IDLE;

    AfxPushLinkage(&surf->swapchain, NIL);

    //AfxInterlinkDeployAsB(&surf->interlinkSentinel, surf);

    surf->idd = NIL;

    if (dctx->vmt->surf && dctx->vmt->surf(surf)) AfxThrowError();
    else
    {
        AfxAssert(surf->vmt);
    }
    return err;
}

_AFX afxClassSpecification const _AfxSurfClassSpec;

afxClassSpecification const _AfxSurfClassSpec =
{
    AFX_FCC_SURF,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxSurface)),
    NIL,
    (void*)_AfxSurfCtor,
    (void*)_AfxSurfDtor,
    .event = _AfxSurfEventHandler,
    .eventFilter = _AfxSurfEventFilter,
    "afxSurface",
    NIL
};
