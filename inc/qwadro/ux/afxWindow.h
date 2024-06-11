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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience on Qwadro                                       //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_WINDOW_H
#define AUX_WINDOW_H

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

typedef struct afxDesktop afxDesktop;

#ifdef _AUX_UX_C
#ifdef _AUX_WINDOW_C
AFX_OBJECT(afxWindow)
{
    afxDesktop*     dwm;
    afxChain        mgrChn;
    afxManager      widMgr;

    afxDrawContext  dctx;
    afxDrawOutput   dout;
    afxDrawOutput   frameDout;
    afxRect         frameRect;
    afxRect         surfaceRect; // aka client area

    struct auxFrame
    {
        afxRect     margin; // used to make room for drop shadow
        afxRect     border; // used to place resize control on frame.
        afxRect     padding; // used to align content area

        //afxInt    left, top, right, bottom;
    };
    afxInt          framePos;

    afxNat          marginL;
    afxNat          marginT;
    afxNat          marginR;
    afxNat          marginB;

    afxString2048   caption;

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

#ifdef _AUX_WINDOW_IMPL
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
#endif//_AUX_WINDOW_IMPL
    void*           udd;
};
#endif//_AUX_WINDOW_C
#endif//_AUX_UX_C

/// The AfxGetWindowRect() function retrieves the dimensions of the bounding rectangle of the specified afxWindow.
/// The dimensions are given in screen coordinates that are relative to the upper-left corner of the screen.

AUX void            AfxGetWindowRect
(
    afxWindow       wnd, /// The afxWindow.
    afxRect*        frame /// The structure that receives the screen coordinates of the upper-left and lower-right corners of the afxWindow.
);

/// The AfxGetSurfaceRect() function retrieves the coordinates of a window's surface (aka client area). 
/// The client coordinates specify the upper-left and lower-right corners of the surface. 
/// Because surface coordinates are relative to the upper-left corner of a window's surface, the coordinates of the upper-left corner are (0,0).

AUX void            AfxGetSurfaceRect
(
    afxWindow       wnd, /// The afxWindow whose surface coordinates are to be retrieved.
    afxRect*        surface /// The structure that receives the surface coordinates.
);

/// The AfxGetWindowDrawOutput() function retrieves the afxDrawOutput for the entire afxWindow, including title bar, menus, and scroll bars. 
/// A afxWindow-owned afxDrawOutput permits painting anywhere in a window, because the origin of the afxDrawOutput is the upper-left corner of the afxWindow instead of the surface (aka client area).

AUX afxBool         AfxGetWindowDrawOutput
(
    afxWindow       wnd, /// The window whose afxDrawOutput is to be retrieved.
    afxDrawOutput*  output /// A placeholder to return a handle to a afxDrawOutput for the specified afxWindow.
);

/// The AfxGetSurfaceDrawOutput() function retrieves a handle to a afxDrawOutput for the surface (aka client area) of a specified afxWindow or for the entire screen. 
/// You can use the returned handle in subsequent draw system functions to draw in the afxDrawOutput. 
/// The afxDrawOutput is an opaque data structure, whose values are used internally by Qwadro Draw I/O System.

AUX afxBool         AfxGetSurfaceDrawOutput
(
    afxWindow       wnd, /// A handle to the afxWindow with a afxDrawOutput that is to be retrieved.
    afxDrawOutput*  output /// A placeholder to return a handle to the afxDrawOutput for the specified afxWindow's surface (aka client area).
);

/// The AfxTraceScreenToSurface() function converts the screen coordinates of a specified point on the screen to surface coordinates.

AUX afxBool         AfxTraceScreenToSurface
(
    afxWindow       wnd, /// The afxWindow whose surface will be used for the conversion.
    afxNat const    screenPos[2], /// A structure that specifies the screen coordinates to be converted.
    afxNat          surfPos[2] /// The new coordinates are relative to the upper-left corner of the specified afxWindow's surface.
);

/// The AfxTraceSurfaceToScreen() function converts the surface coordinates of a specified point to screen coordinates.

AUX afxBool         AfxTraceSurfaceToScreen
(
    afxWindow       wnd, /// The afxWindow whose surface is used for the conversion.
    afxNat const    surfPos[2], /// A structure that contains the surface coordinates to be converted.
    afxNat          screenPos[2] /// The new screen coordinates are copied into this structure if the function succeeds.
);

/// Changes the text of the specified afxWindow's title bar (if it has one). 
/// If the specified afxWindow is a control, the text of the control is changed. 
/// However, AfxFormatWindowCaption() cannot change the text of a control in another application.

AUX afxNat          AfxFormatWindowCaption
(
    afxWindow       wnd, /// The afxWindow whose text is to be changed.
    afxChar const*  fmt, /// The new title or control text.
    ...
);

/// The AfxRedrawWindow() function updates the specified rectangle or region in a window's surface (aka client area).

AUX afxError        AfxRedrawWindow
(
    afxWindow       wnd,
    afxRect const*  surr
);

AUX void            AfxStepWindow
(
    afxWindow       wnd,
    afxReal64*      ct,
    afxReal64*      dt
);

/// Sets the upper left position of the afxWindow (point) including its window frame.

AUX afxBool         AfxMoveWindow(afxWindow wnd, afxNat const pos[2]);

AUX afxError        AfxAdjustWindow(afxWindow wnd, afxRect const* frame);
AUX afxError        AfxAdjustWindowFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent);

AUX afxError        AfxAdjustWindowSurface(afxWindow wnd, afxRect const* area);
AUX afxError        AfxAdjustWindowSurfaceFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent);

/// Sets the afxWindow's icon in the Shell.

AUX afxError        AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri);
AUX afxError        AfxChangeWindowIcon(afxWindow wnd, afxTarga* tga);

AUX afxManager*     AfxGetWidgetClass(afxWindow wnd);
AUX afxNat          AfxEnumerateWidgets(afxWindow wnd, afxNat first, afxNat cnt, afxWidget widgets[]);

////////////////////////////////////////////////////////////////////////////////

AUX afxError        AfxAcquireWindow(afxNat dwmId, afxDrawContext dctx, afxRect const* rc, afxWindow* window);
AUX afxError        AfxAcquireChildWindow(afxWindow wnd, afxRect const* pos);

AUX afxManager*     AfxGetWindowClass(void);

#endif//AUX_WINDOW_H
