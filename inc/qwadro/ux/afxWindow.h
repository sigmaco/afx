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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// Advanced User Experience Extensions for Qwadro

#ifndef AFX_WINDOW_H
#define AFX_WINDOW_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/math/afxViewport.h"

// our facade

AFX_DEFINE_STRUCT(afxStyle)
{
    int a;
};

AFX_DEFINE_STRUCT(afxLayout)
{
    struct
    {
        afxRect margin;
        afxRect border;
        afxRect padding;
        afxNat  wh[2];
    };
};

#ifdef _AUX_UX_C
#ifdef _AFX_WINDOW_C
AFX_OBJECT(afxWindow)
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

    afxClock        startClock;
    afxClock        lastClock;

    afxV2d          cursorPos;
    afxV2d          cursorMove;
    afxV2d          cursorPosNdc;
    afxV2d          cursorMoveNdc;

    afxBool         cursorDisabled;
    afxBool         cursorConfined;

    afxWidget       hoveredWidg;
    afxV2d          hoveredPoint;
    afxWidget       focusedWidg;
    afxWidget       grabbedWidg;
    afxV2d          grabPoint;

#ifdef _AFX_WINDOW_IMPL
#ifdef AFX_OS_WIN
    struct
    {
        HWND        hWnd;
        HDC         hFrameDc;
        HDC         hSurfaceDc;
        HICON       hIcon;
        afxAtomic   lastBufIdx;
        afxBool     swap;
    }               w32;
#endif//AFX_OS_WIN
#endif//_AFX_APPLICATION_IMPL
    void*           udd;
};
#endif//_AFX_WINDOW_C
#endif//_AUX_UX_C


AUX afxError        AfxAdjustWindow(afxWindow wnd, afxWhd const whd);
AUX afxError        AfxAdjustWindowNdc(afxWindow wnd, afxV3d const whd);

AUX afxNat          AfxFormatWindowCaption(afxWindow wnd, afxChar const* fmt, ...);

AUX afxError        AfxRedrawWindow(afxWindow wnd);
AUX void            AfxStepWindow(afxWindow wnd, afxReal64* ct, afxReal64* dt);

AUX afxBool         AfxGetWindowDrawOutput(afxWindow wnd, afxDrawOutput* output);

AUX afxError        AfxChangeWindowIcon(afxWindow wnd, afxTarga* tga);

AUX afxManager*     AfxGetWidgetClass(afxWindow wnd);
AUX afxNat          AfxEnumerateWidgets(afxWindow wnd, afxNat first, afxNat cnt, afxWidget widgets[]);

////////////////////////////////////////////////////////////////////////////////

AUX afxError        AfxAcquireWindow(afxDrawContext dctx, afxWhd const whd, afxWindow* window);

AUX afxManager*     AfxGetWindowClass(void);

#endif//AFX_WINDOW_H
