/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SURFACE_C
#define _AFX_TEXTURE_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#include "../_classified/afxDrawClassified.h"

extern struct _afxDsysD TheDrawSystem;

_AFX afxSurfaceState AfxGetSurfaceState(afxSurface surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &surf, AFX_FCC_SURF);

    return surf->state;
}

_AFX afxTexture AfxGetSurfaceTexture(afxSurface surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &surf, AFX_FCC_SURF);
    return surf->tex;
}

_AFX afxBool AfxSurfaceIsPresentable(afxSurface surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &surf, AFX_FCC_SURF);
    return surf->state == AFX_SURF_STATE_PRESENTABLE;
}

_AFX afxError AfxAcquireSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    AfxAssert(usage & AFX_TEX_USAGE_DRAW);
    AfxEntry("fmt=%u,whd=[%u,%u,%u],usage=%u", fmt, extent[0], extent[1], extent[2], usage);
    
    usage &= AFX_TEX_USAGE_DRAW;

    if (fmt >= AFX_PFD_S8 && fmt <= AFX_PFD_D32FS8)
        usage |= AFX_TEX_USAGE_DRAW;
    else
        usage |= AFX_TEX_USAGE_DRAW;

    afxTextureBlueprint texb;
    AfxAcquireTextureBlueprint(&texb, extent, fmt, usage);
    AfxTextureBlueprintAddImage(&texb, fmt, extent, NIL, NIL);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    if (AfxAcquireObjects(&dctx->surfaces, cnt, (afxHandle*)surf, (void*[]) { (void*)&texb }))
        AfxThrowError();

    AfxTextureBlueprintEnd(&texb, 0, NIL);

    return err;
};

_AFX afxNat AfxEnumerateSurfaces(afxDrawContext dctx, afxNat first, afxNat cnt, afxSurface surf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(surf);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    return AfxEnumerateInstances(&dctx->surfaces, first, cnt, (afxHandle*)surf);
}
