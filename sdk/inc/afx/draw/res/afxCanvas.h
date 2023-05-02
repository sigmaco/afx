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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_CANVAS_H
#define AFX_CANVAS_H

#include "afx/draw/res/afxSurface.h"

AFX_DEFINE_HANDLE(afxCanvas);

AFX_DEFINE_STRUCT(afxSurfaceSpecification)
{
    afxSurface          surf;
    afxPixelFormat      fmt;
    afxTextureUsage     usage;
};

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxCanvas) // our concept of a framebuffer
{
    afxObject           obj;
};

#endif

// An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.

AFX void*               AfxCanvasGetContext(afxCanvas canv);
AFX void*               AfxCanvasGetDriver(afxCanvas canv);
AFX void*               AfxCanvasGetDrawSystem(afxCanvas canv);

AFX afxNat*             AfxCanvasGetExtent(afxCanvas canv, afxWhd extent);
AFX afxResult           AfxCanvasSetExtent(afxCanvas canv, afxWhd const extent);

AFX afxNat              AfxCanvasGetSurfaceCount(afxCanvas canv);
AFX afxSurface          AfxCanvasGetSurface(afxCanvas canv, afxNat idx);

#endif//AFX_CANVAS_H