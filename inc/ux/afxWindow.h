/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

/*
    In Qwadro, a window is a graphical elements that allow users to interact with software applications. 
    The concept of window is fundamental to modern graphical user interfaces (GUIs), such as those used in desktop environments. 
    Each window typically represents a different application or document and can be resized, minimized, maximized, or closed. 
    Windows enable multitasking by displaying multiple applications on the screen at once.
    
    Multiple windows can be opened and managed simultaneously. Often includes the title of the application or document.
    Common window controls include minimize, maximize, and close buttons. You can adjust the size of a window to your preference.

    The surface is the client area within the window frame where the content or functionality is displayed.
    The frame is the decoration of a window, usually including borders, title bar and control buttons on it.
*/

#ifndef AUX_WINDOW_H
#define AUX_WINDOW_H

#include "qwadro/inc/ux/afxUxDefs.h"
#include "qwadro/inc/math/avxViewport.h"

// our facade

typedef enum afxWindowFlag
{
    afxWindowFlag_MINIMIZABLE,
    afxWindowFlag_MAXIMIZABLE,
    afxWindowFlag_RESIZABLE,
    afxWindowFlag_IMMERSIVE,
} afxWindowFlags;

AFX_DEFINE_STRUCT(afxWindowConfig)
{
    afxWindowFlags      flags;
    afxString           title;
    afxBool             (*eventCb)(afxWindow, auxEvent*);
    afxDrawSystem       dsys;
    afxSurfaceConfig    dout;
    afxInt              atX;
    afxInt              atY;
    void*               udd;
    afxString           tag;
};

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
        afxUnit  wh[2];
    };
};

/*
    The AfxGetWindowRect() function retrieves the dimensions of the bounding rectangle of the specified afxWindow. 
    The dimensions are given in screen coordinates that are relative to the upper-left corner of the screen.

    The AfxGetSurfaceRect() function retrieves the coordinates of a window's surface (aka client area). 
    The client coordinates specify the upper-left and lower-right corners of the surface. 
    Because surface coordinates are relative to the upper-left corner of a window's surface, the coordinates of the upper-left corner are (0,0).
*/

AUX void            AfxGetWindowRect
(
    afxWindow       wnd, 
    afxRect*        frame, 
    afxRect*        surface
);

AUX afxError        AfxAdjustWindow
(
    afxWindow       wnd, 
    afxRect const*  area
);

/*
    The AfxGetWindowSurface() function retrieves a handle to a afxSurface for the surface (aka client area) of a specified afxWindow or for the entire screen. 
    You can use the returned handle in subsequent draw system functions to draw in the afxSurface. 
    The afxSurface is an opaque data structure, whose values are used internally by Qwadro Draw I/O System.
*/

AUX afxError AfxGetWindowSurface(afxWindow wnd, afxSurface* surface);

/*
    The AfxTraceScreenToSurface() function converts the screen coordinates of a specified point on the screen to surface coordinates.
*/

AUX afxBool AfxTraceScreenToSurface(afxWindow wnd, afxUnit const screenPos[2], afxUnit surfPos[2]);

/*
    The AfxTraceSurfaceToScreen() function converts the surface coordinates of a specified point to screen coordinates.
*/

AUX afxBool AfxTraceSurfaceToScreen(afxWindow wnd, afxUnit const surfPos[2], afxUnit screenPos[2]);

/*
    The AfxMakeWindowCursory() method makes a window cursory, that is, superficial, lacking in depth.
    At this mode, which is the default one, the cursor is enabled to work with widgets and other interactive elements.
    Disabling cursory may be used to grab and/or center the mouse to be used in window surface as, for example, a interactive aim.
*/

AUX afxError AfxMakeWindowCursory(afxWindow wnd, afxRect const* confinement, afxBool cursory);

/*
    Changes the text of the specified afxWindow's title bar (if it has one). 
    If the specified afxWindow is a control, the text of the control is changed. 
    However, AfxFormatWindowTitle() cannot change the text of a control in another application.
*/

AUX afxUnit AfxFormatWindowTitle(afxWindow wnd, afxChar const* fmt, ...);

/*
    The AfxRedrawWindow() function updates the specified rectangle or region in a window's surface (aka client area).
*/

AUX afxError AfxRedrawWindow(afxWindow wnd, afxRect const* area);

AUX afxError AfxRedrawWidgets(afxWindow wnd, afxDrawContext dctx);

//AUX void AfxStepWindow(afxWindow wnd, afxReal64* ct, afxReal64* dt);

/*
    Sets the afxWindow's icon in the Shell.
*/

AUX afxError        AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri);
AUX afxError        AfxChangeWindowIcon(afxWindow wnd, avxRaster ras, avxRasterRegion const* rgn);

AUX afxError        AfxLoadWindowCursor(afxWindow wnd, afxUri const* uri);
AUX afxError        AfxChangeWindowCursor(afxWindow wnd, avxRaster curs, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY);

AUX void*           AfxGetWindowUdd(afxWindow wnd);

////////////////////////////////////////////////////////////////////////////////

AUX afxError        AfxConfigureWindow(afxWindowConfig* cfg, afxV2d const origin, afxV2d const extent);

AUX afxError        AfxAcquireWindow(afxWindowConfig const* cfg, afxWindow* window);
AUX afxError        AfxAcquireChildWindow(afxWindow wnd, afxRect const* pos);

#endif//AUX_WINDOW_H
