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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_SURFACE_C
#define _AFX_RASTER_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#include "afx/draw/afxDrawContext.h"

extern struct _afxDsysD TheDrawSystem;

_AFX afxRaster AfxGetSurfaceTexture(afxSurface surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &surf, afxFcc_SURF);
    return surf->tex;
}

_AFX afxError AfxAcquireSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    AfxAssert(usage & afxRasterFlag_DRAW);
    AfxEntry("fmt=%u,whd=[%u,%u,%u],usage=%u", fmt, extent[0], extent[1], extent[2], usage);
    
    usage &= afxRasterFlag_DRAW;

    if (fmt >= AFX_PFD_S8 && fmt <= AFX_PFD_D32FS8)
        usage |= afxRasterFlag_DRAW;
    else
        usage |= afxRasterFlag_DRAW;

    afxRasterInfo texi = { 0 };
    texi.whd[0] = extent[0];
    texi.whd[1] = extent[1];
    texi.whd[2] = extent[2];
    texi.fmt = fmt;
    texi.usage = usage | afxRasterFlag_DRAW;
    texi.layerCnt = 1;

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (AfxAcquireObjects(&dctx->surfaces, cnt, (afxHandle*)surf, (void*[]) { (void*)&texi }))
        AfxThrowError();

    return err;
};