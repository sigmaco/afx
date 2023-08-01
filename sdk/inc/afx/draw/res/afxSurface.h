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

#ifndef AFX_SURFACE_H
#define AFX_SURFACE_H

#include "afx/draw/res/afxTexture.h"

typedef enum afxSurfaceState
{
    AFX_SURF_STATE_IDLE,
    AFX_SURF_STATE_PRESENTABLE,
    AFX_SURF_STATE_PAINTING, // open
    AFX_SURF_STATE_PENDING // submitted    
} afxSurfaceState;

AFX_DECLARE_STRUCT(_afxSurfVmt);

#if (defined(_AFX_SURFACE_C) && !defined(_AFX_TEXTURE_C))
#   error "afxTexture not exposed"
#endif

AFX_OBJECT(afxSurface) // our concept of a framebuffer
{
    AFX_OBJECT(afxTexture)  tex;
    _afxSurfVmt const*      vmt;
    void*                   idd; // implementation-defined data
#ifdef _AFX_SURFACE_C
    afxSurfaceState         state;
    afxLinkage              swapchain; // if state == PENDING, must have a swapchain linked to this canvas. Used by draw context on canvas presentation to surface.
#endif
};

AFX afxDrawContext          AfxGetSurfaceContext(afxSurface surf);

AFX afxSurfaceState         AfxGetSurfaceState(afxSurface surf);
AFX afxBool                 AfxSurfaceIsPresentable(afxSurface surf);

#endif//AFX_SURFACE_H