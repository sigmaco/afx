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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_SURFACE_H
#define AFX_SURFACE_H

#include "afx/draw/afxRaster.h"
#include "afx/draw/afxDrawCommands.h"

typedef enum afxSurfaceState
{
    AFX_SURF_STATE_IDLE,
    AFX_SURF_STATE_PRESENTABLE,
    AFX_SURF_STATE_PAINTING, // open
    AFX_SURF_STATE_PENDING // submitted    
} afxSurfaceState;

#ifdef _AFX_DRAW_C
#ifdef _AFX_SURFACE_C
AFX_OBJECT(afxSurface)
#else
struct afxBaseSurface
#endif
{
    afxRaster          tex; /// the texture subresource that will be output to for this color attachment.
    afxSurfaceLoadOp    loadOp; /// Indicates the load operation to perform on view prior to executing the render pass.
    afxSurfaceStoreOp   storeOp; /// The store operation to perform on view after executing the render pass.
    afxClearValue       clearValue; /// Indicates the value to clear view to prior to executing the render pass.
    afxRaster          resolve; /// the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
};
#endif

AFX afxError                AfxAcquireRasterSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage);
AFX afxError                AfxAcquireDepthSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage);

AFX afxError                AfxAcquireSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage);

AFX afxRaster              AfxGetSurfaceTexture(afxSurface surf);

#endif//AFX_SURFACE_H
