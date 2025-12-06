/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   U S E R   E X P E R I E N C E   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_WINDOW_DDK_H
#define AUX_WINDOW_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

AFX_DECLARE_STRUCT(_auxIddWnd);

#ifndef _AUX_UX_C
AFX_DECLARE_STRUCT(_auxDdiWnd);
#else
AFX_DEFINE_STRUCT(_auxDdiWnd)
{
    afxError(*damageCb)(afxWindow, afxRect const*);
    afxError(*redrawCb)(afxWindow, afxRect const*);
    afxError(*chIconCb)(afxWindow, avxRaster, avxRasterRegion const* rgn);
    afxError(*chCursCb)(afxWindow, avxRaster, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY);
    afxError(*adjustCb)(afxWindow, afxAnchor, afxRect*);
    void(*focus)(afxWindow);
    afxBool(*hasFocus)(afxWindow);
    afxUnit(*titleCb)(afxWindow);
};
#endif

#ifdef _AUX_WINDOW_C
#ifdef _AUX_WINDOW_IMPL
AFX_OBJECT(_auxWindow)
#else
AFX_OBJECT(afxWindow)
#endif
{
    _auxIddWnd*         idd;
    _auxDdiWnd const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxDesktop*         dwm;
    afxChain            classes;
    afxClass            widCls;

    // .adjusting is key to prevent recursion.
    afxBool             adjusting;
    afxReal             aspRatio;
    afxAnchor           anchor;
    afxRect             screenRc;
    afxSurface          frameDout;
    afxInt              framePos;
    afxRect             frameRc;
    afxRect             frameRcMin;
    afxRect             frameRcMax;
    afxRect             frameMargin; // used to make room for drop shadow
    afxUnit             frameMarginL;
    afxUnit             frameMarginT;
    afxUnit             frameMarginR;
    afxUnit             frameMarginB;
    afxRect             frameBorder; // used to place resize control on frame.
    afxSurface          surfaceDout;
    afxRect             surfacePadding; // used to align content area
    afxRect             surfaceRc; // aka client area
    
    avxRaster           icon;
    avxRasterRegion     iconCrop;
    avxRaster           curs;
    avxRasterRegion     cursCrop;
    afxString512        title;

    afxBool             alwaysOnTop;
    afxBool             active;
    afxBool             focused;
    afxBool             decorated;
    afxBool             floating;
    afxBool             resizable;
    afxBool             fullscreen;

    afxClock            startClock;
    afxClock            lastClock;

    afxV2d              cursPos;
    afxV2d              cursMove;
    afxV2d              cursPosNdc;
    afxV2d              cursMoveNdc;
    afxBool             cursConfined; // confined in window
    afxRect             cursConfinRect;
    afxBool             cursHidden; // visually disabled in window surface
    
    afxBool             redrawFrameRequested;
    afxBool             redrawSurfaceRequested;

    afxWidget           hoveredWidg;
    afxV2d              hoveredPoint;
    afxWidget           focusedWidg;
    afxWidget           grabbedWidg;
    afxV2d              grabPoint;
};
#endif//_AUX_WINDOW_C

AUX afxClassConfig const _AUX_WND_CLASS_CONFIG;
AUX _auxDdiWnd const _AUX_DDI_WND;

AUX afxClass const* _AuxWndGetWidClass(afxWindow wnd);

AUX afxUnit _AfxWndFormatTitleCb(afxWindow wnd);
AUX afxError _AfxWndAdjustCb(afxWindow wnd, afxAnchor, afxRect* c);

#endif//AUX_WINDOW_DDK_H
