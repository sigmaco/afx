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

#include "afx/draw/afxTexture.h"
#include "afxDrawCommands.h"

typedef enum afxSurfaceState
{
    AFX_SURF_STATE_IDLE,
    AFX_SURF_STATE_PRESENTABLE,
    AFX_SURF_STATE_PAINTING, // open
    AFX_SURF_STATE_PENDING // submitted    
} afxSurfaceState;

AFX_DECLARE_STRUCT(_afxSurfVmt);

#ifdef _AFX_SURFACE_C
AFX_OBJECT(afxSurface)
#else
struct afxBaseSurface
#endif
{
    afxTexture          tex; /// the texture subresource that will be output to for this color attachment.
    afxSurfaceLoadOp    loadOp; /// Indicates the load operation to perform on view prior to executing the render pass.
    afxSurfaceStoreOp   storeOp; /// The store operation to perform on view after executing the render pass.
    afxClearValue       clearValue; /// Indicates the value to clear view to prior to executing the render pass.
    afxTexture          resolve; /// the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
};

AFX afxError                AfxAcquireRasterSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage);
AFX afxError                AfxAcquireDepthSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage);

AFX afxError                AfxAcquireSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage);

AFX afxTexture              AfxGetSurfaceTexture(afxSurface surf);

#endif//AFX_SURFACE_H