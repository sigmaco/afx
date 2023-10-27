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

// This section is part of SIGMA GL.

#ifndef AFX_SURFACE_H
#define AFX_SURFACE_H

#include "afx/draw/afxTexture.h"

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
    afxTexture              tex;
    afxSurfaceState         state;
    afxLinkage              swapchain; // if state == PENDING, must have a swapchain linked to this canvas. Used by draw context on canvas presentation to surface.
};

AFX afxError                AfxAcquireSurfaces(afxDrawContext dctx, afxNat cnt, afxSurface surf[], afxWhd const extent, afxPixelFormat fmt, afxFlags usage);

AFX afxSurfaceState         AfxGetSurfaceState(afxSurface surf);
AFX afxBool                 AfxSurfaceIsPresentable(afxSurface surf);

AFX afxTexture              AfxGetSurfaceTexture(afxSurface surf);

#endif//AFX_SURFACE_H