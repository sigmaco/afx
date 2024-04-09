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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// Advanced User Experience Extensions for Qwadro

#ifndef AFX_OVERLAY_H
#define AFX_OVERLAY_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/math/afxViewport.h"

#ifdef _AFX_UX_C
#ifdef _AFX_OVERLAY_C
AFX_OBJECT(afxOverlay)
{
    afxChain        mgrChn;
    afxManager      widMgr;

    afxDrawContext  dctx;
    afxDrawOutput   dout;
    afxWhd          whd;

    afxRestring     caption;

    afxBool         active;
    afxBool         focused;
    afxBool         decorated;
    afxBool         floating;
    afxBool         fullscreen;

    afxV2d          cursorPos;
    afxV2d          cursorMove;
    afxV2d          cursorPosNdc;
    afxV2d          cursorMoveNdc;

    afxWidget       hoveredWidg;
    afxV2d          hoverPoint;
    afxWidget       focusedWidg;
    afxWidget       grabbedWidg;
    afxV2d          grabPoint;
#ifdef _AFX_OVERLAY_IMPL
#ifdef AFX_PLATFORM_WIN
    struct
    {
        WNDCLASSEX  wndClss;
        HWND        hWnd;
        HDC         hDc;
        afxAtomic   lastBufIdx;
        afxBool     swap;
    }               w32;
#endif//AFX_PLATFORM_WIN
#endif//_AFX_APPLICATION_IMPL
    void*           udd;
};
#endif//_AFX_OVERLAY_C
#endif//_AFX_UX_C


AUX afxError        AfxDoOverlay(afxOverlay ovy);

AUX afxError        AfxAdjustOverlay(afxOverlay ovly, afxWhd const whd);
AUX afxError        AfxAdjustOverlayNdc(afxOverlay ovy, afxV3d const whd);

AUX afxNat          AfxFormatOverlayCaption(afxOverlay ovly, afxChar const* fmt, ...);

AUX afxManager*     AfxGetWidgetClass(afxOverlay ovly);

////////////////////////////////////////////////////////////////////////////////

AUX afxError        AfxAcquireOverlay(afxDrawContext dctx, afxWhd const whd, afxOverlay* overlay);

AUX afxManager*     AfxGetOverlayClass(void);

#endif//AFX_OVERLAY_H
