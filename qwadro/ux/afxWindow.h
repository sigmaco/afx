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

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_WINDOW_H
#define AUX_WINDOW_H

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

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxWidget.h"

// our facade

typedef enum afxAnchor
{
    afxAnchor_LEFT      = AFX_BITMASK(0),
    afxAnchor_CENTER    = AFX_BITMASK(1),
    afxAnchor_RIGHT     = AFX_BITMASK(2),
    afxAnchor_TOP       = AFX_BITMASK(3),
    afxAnchor_MIDDLE    = AFX_BITMASK(4),
    afxAnchor_BOTTOM    = AFX_BITMASK(5),
    afxAnchor_V         = (afxAnchor_LEFT | afxAnchor_CENTER | afxAnchor_RIGHT),
    afxAnchor_H         = (afxAnchor_TOP | afxAnchor_MIDDLE | afxAnchor_BOTTOM),
    afxAnchor_LT        = (afxAnchor_LEFT | afxAnchor_TOP),
    afxAnchor_LM        = (afxAnchor_LEFT | afxAnchor_MIDDLE),
    afxAnchor_LB        = (afxAnchor_LEFT | afxAnchor_BOTTOM),
    afxAnchor_RT        = (afxAnchor_RIGHT | afxAnchor_TOP),
    afxAnchor_RM        = (afxAnchor_RIGHT | afxAnchor_MIDDLE),
    afxAnchor_RB        = (afxAnchor_RIGHT | afxAnchor_BOTTOM),
    afxAnchor_CT        = (afxAnchor_CENTER | afxAnchor_TOP),
    afxAnchor_CM        = (afxAnchor_CENTER | afxAnchor_MIDDLE),
    afxAnchor_CB        = (afxAnchor_CENTER | afxAnchor_BOTTOM),
} afxAnchor;

typedef enum afxWindowFlag
// Flags specifying behavior and characteristics for windows.
{
    // The window can be minimized by frame control.
    afxWindowFlag_MINIMIZABLE   = AFX_BITMASK(0),
    // The window can be maximized by frame control.
    afxWindowFlag_MAXIMIZABLE   = AFX_BITMASK(1),
    // The window can be resized by frame control.
    afxWindowFlag_RESIZABLE     = AFX_BITMASK(2),
    // The window can take full control of the display port.
    afxWindowFlag_IMMERSIVE     = AFX_BITMASK(3),
} afxWindowFlags;

typedef enum afxWindowEdge
{
    afxWindowEdge_L     = AFX_BITMASK(0),
    afxWindowEdge_T     = AFX_BITMASK(1),
    afxWindowEdge_R     = AFX_BITMASK(2),
    afxWindowEdge_B     = AFX_BITMASK(3),
    afxWindowEdge_LT    = (afxWindowEdge_L | afxWindowEdge_T),
    afxWindowEdge_LB    = (afxWindowEdge_L | afxWindowEdge_B),
    afxWindowEdge_RT    = (afxWindowEdge_R | afxWindowEdge_T),
    afxWindowEdge_RB    = (afxWindowEdge_R | afxWindowEdge_B),
} afxWindowEdge;

typedef enum afxWindowState
{
    afxWindowState_RESIZING = 3,
    afxWindowState_ACTIVATED = 4,
    afxWindowState_MAXIMIZED = 1,
    afxWindowState_FSE = 2,
    afxWindowState_TILED_L = 5,
    afxWindowState_TILED_R = 6,
    afxWindowState_TILED_T = 7,
    afxWindowState_TILED_B = 8,

    afxWindowState_SUSPENDED,
    afxWindowState_MINIMIZED
} afxWindowState;

AFX_DEFINE_STRUCT(afxWindowInfo)
{
    // The bitmask specifying behavior and characteristics of the window.
    afxWindowFlags      flags;
    // The event handler callback assigned to the window.
    afxBool             (*eventCb)(afxWindow, auxEvent*);
    afxRect             frame;
        // The afxRect structure which will be filled with the geometry of the window surface.
    afxRect             surface;
    // The drawable surface for the window.
    afxSurface          dout;
    // A user-defined data.
    void*               udd;
    // An debugging string.
    afxString           tag;
};

AFX_DEFINE_STRUCT(afxWindowConfig)
// The strucuture specifying the configuration of a window.
{
    // The bitmask specifying behavior and characteristics of the window.
    afxWindowFlags      flags;
    // A string specifying the main caption for the window.
    afxString           title;
    // The event handler callback assigned to the window.
    afxBool             (*eventCb)(afxWindow, auxEvent*);
    // The draw system providing the drawable surface and resources for the window.
    afxDrawSystem       dsys;
    // The configuration of the drawable surface.
    afxSurfaceConfig    dout;
    // Optional. The display port to which the window will be open on.
    afxDisplay          disp;
    afxUnit             dport;
    // 
    afxAnchor           anchor;
    // The origin of the window. If display port is specified, it is related to the area covered by the display port, 
    // else case it is related to the desktop coordinate system (which can be negative).
    afxInt              x, y;
    // A user-defined data.
    void*               udd;
    // An debugging string.
    afxString           tag;
};

AUX afxError AfxConfigureWindow
(
    // The session which the window will be into.
    afxEnvironment env,
    // A structure specifying the configuration for the window.
    afxWindowConfig* cfg, 
    // The optional desired origin in NDC.
    afxV2d const origin, 
    // The optional desired extent in NDC.
    afxV2d const extent
);

AUX afxError AfxAcquireWindow
(
    // The session which the window will be into.
    afxEnvironment env,
    // A structure specifying the configuration for the window.
    afxWindowConfig const* cfg, 
    // A placeholder to receive the handle of the acquired window.
    afxWindow* window
);

AUX afxBool AFX_WND_EVENT_HANDLER(afxWindow wnd, auxEvent *ev);

////////////////////////////////////////////////////////////////////////////////

/*
    The AfxGetWindowRect() function retrieves the dimensions of the bounding rectangle of the specified afxWindow. 
    The dimensions are given in screen coordinates that are relative to the upper-left corner of the screen.

    The AfxGetSurfaceRect() function retrieves the coordinates of a window's surface (aka client area). 
    The client coordinates specify the upper-left and lower-right corners of the surface. 
    Because surface coordinates are relative to the upper-left corner of a window's surface, the coordinates of the upper-left corner are (0,0).
*/

AUX afxBool         AfxGetWindowRect
(
    // The handle of the afxWindow.
    afxWindow       wnd, 
    // 
    afxAnchor       anchor, 
    // The afxRect structure which will be filled with the geometry of the window surface.
    afxRect*        surface
);

/*
    AfxAdjustWindow adjusts the position and size of a window (afxWindow) according to:
     - An optional display (afxDisplay)
     - A display port (afxUnit), usually representing a specific monitor/viewport inside the display
     - An anchor (afxAnchor) such as LEFT, CENTER, MIDDLE, BOTTOM, etc.
     - A desired surface/geometry (afxRect const* surface)
    
    If a valid display handle is provided, the window's geometry is interpreted relative to the display port 
    (i.e., relative to the monitor or workspace area).

    If the display is not valid, the adjustment is done in the window's parent coordinate space or a default 
    global coordinate space (depending on the framework's rules).
*/

AUX afxError        AfxAdjustWindow
(
    // The handle of the afxWindow.
    afxWindow       wnd,
    // An optional afxDisplay handle.
    afxDisplay      disp,
    // When afxDisplay is used, a port (a specific monitor/viewport) inside the display must be specified.
    afxUnit         dport,
    // The anchor determining how the geometry will be positioned.
    afxAnchor       anchor,
    // The geometry (x, y, width, height) desired for the window.
    // If a valid afxDisplay handle is provided, the geometry is interpreted relative to the display port 
    // (i.e., relative to the monitor or workspace area).
    afxRect const*  surface
);

/*
    The AfxGetWindowSurface() function retrieves a handle to a afxSurface for the surface (aka client area) of a specified afxWindow or for the entire screen. 
    You can use the returned handle in subsequent draw system functions to draw in the afxSurface. 
    The afxSurface is an opaque data structure, whose values are used internally by Qwadro Draw I/O System.
*/

AUX afxError AfxGetWindowSurface
(
    // The handle of the afxWindow owning the surface.
    afxWindow wnd, 
    // A placeholder which will hold the handle of the afxSurface object.
    afxSurface* surface
);

/*
    The AfxGetOnSurfaceScreenPosition() function converts the screen coordinates of a specified point on the screen to surface coordinates.
*/

AUX afxBool AfxGetOnSurfaceScreenPosition
(
    // The handle of the afxWindow owning the surface.
    afxWindow wnd,
    afxUnit const screenPos[2],
    afxUnit surfPos[2]
);

/*
    The AfxGetOnScreenSurfacePosition() function converts the surface coordinates of a specified point to screen coordinates.
*/

AUX afxBool AfxGetOnScreenSurfacePosition
(
    // The handle of the afxWindow owning the surface.
    afxWindow wnd, 
    // The coordinates in window's surface area.
    afxUnit const surfPos[2], 
    // A variable which will hold the resolved coordinates in screen space.
    afxUnit screenPos[2]
);

/*
    The AfxMakeWindowCursory() method makes a window cursory, that is, superficial, lacking in depth.
    At this mode, which is the default one, the cursor is enabled to work with widgets and other interactive elements.
    Disabling cursory may be used to grab and/or center the mouse to be used in window surface as, for example, a interactive aim.
*/

AUX afxError AfxMakeWindowCursory
(
    // The handle of the afxWindow owning the surface.
    afxWindow wnd, 
    // A optional afxRect structure limiting the surface subarea of confinement.
    afxRect const* confinement, 
    // A boolean key specifying whether "on surface" or "in surface".
    afxBool cursory
);

/*
    Changes the text of the specified afxWindow's title bar (if it has one). 
    If the specified afxWindow is a control, the text of the control is changed. 
    However, AfxFormatWindowTitle() cannot change the text of a control in another application.
*/

AUX afxUnit AfxFormatWindowTitle
(
    // The handle of the afxWindow to be renamed.
    afxWindow wnd, 
    // The string specifying text and/or format for the caption.
    afxChar const* fmt, 
    // Optional arguments to the formattation string.
    ...
);

/*
    The AfxDamageWindow() function invalidates the specified rectangle or region in a window's surface (aka client area).
    By calling this method, a redrawn request event will be provoked.
*/

AUX afxError AfxDamageWindow
(
    // The handle of the afxWindow to be redrawn.
    afxWindow wnd,
    // An optinal afxRect limiting an area of interest to be redrawn.
    afxRect const* area
);

/*
    The AfxRedrawWindow() function updates the specified rectangle or region in a window's surface (aka client area).
*/

AUX afxError AfxRedrawWindow
(
    // The handle of the afxWindow to be redrawn.
    afxWindow wnd, 
    // An optinal afxRect limiting an area of interest to be redrawn.
    afxRect const* area
);

AUX afxError AfxRedrawWidgets
(
    // The handle of the afxWindow to be redrawn.
    afxWindow wnd, 
    // An optional afxRect limiting an area of interest where widgets will be redrawn.
    afxRect const* area, 
    // The afxDrawContext to be used to store the commands generated.
    afxDrawContext dctx
);

//AUX void AfxStepWindow(afxWindow wnd, afxReal64* ct, afxReal64* dt);

/*
    Sets the afxWindow's icon in the Shell.
*/

AUX afxError        AfxLoadWindowIcon
(
    // The handle of the afxWindow to be redecorated.
    afxWindow wnd, 
    // The URI specifying the location of the file containing the icon art.
    afxUri const* uri
);

AUX afxError        AfxChangeWindowIcon
(
    // The handle of the afxWindow to be redecorated.
    afxWindow wnd, 
    // The handle of a avxRaster used as source for the icon.
    avxRaster ras, 
    // The region of the specified raster specifying the icon.
    avxRasterRegion const* rgn
);

AUX afxError        AfxLoadWindowCursor
(
    // The handle of the afxWindow to be redecorated.
    afxWindow wnd,
    // The URI specifying the location of the file containing the cursor art.
    afxUri const* uri
);

AUX afxError        AfxChangeWindowCursor
(
    // The handle of the afxWindow to be redecorated.
    afxWindow wnd,
    // The handle of a avxRaster used as source for the cursor.
    avxRaster curs,
    // The region of the specified raster specifying the cursor.
    avxRasterRegion const* rgn, 
    // The horizontal displacement for the cursor's affection point.
    afxInt hotspotX, 
    // The vertical displacement for the cursor's affection point.
    afxInt hotspotY
);

AUX void*           AfxGetWindowUdd
(
    // The handle of the afxWindow.
    afxWindow wnd
);

AUX afxError AfxCloseWindow(afxWindow wnd);
AUX afxError AfxRestoreWindow(afxWindow wnd);
AUX afxError AfxMaximizeWindow(afxWindow wnd);
AUX afxError AfxMinimizeWindow(afxWindow wnd);

#endif//AUX_WINDOW_H
