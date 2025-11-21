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
 // Advanced User Experience on Qwadro                                       //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_UX_DEFS_H
#define AUX_UX_DEFS_H

#include "qwadro/exec/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/mix/afxMixSystem.h"

#ifndef __e2mmux__
#   ifdef _DEBUG
#       define AUX DLLIMPORT extern 
#       define AUXINL DLLIMPORT EMBED
#   else
#       define AUX DLLIMPORT extern 
#       define AUXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _AUX DLLEXPORT
#       define AUX DLLEXPORT extern 
#       define _AUXINL DLLEXPORT INLINE
#       define AUXINL DLLEXPORT EMBED
#   else
#       define _AUX DLLEXPORT
#       define AUX DLLEXPORT extern 
#       define _AUXINL DLLEXPORT INLINE
#       define AUXINL DLLEXPORT EMBED
#   endif
#endif//__e2mmux__

AFX_DEFINE_HANDLE(afxWidget);
AFX_DEFINE_HANDLE(afxScript);
AFX_DEFINE_HANDLE(afxEnvironment);
AFX_DEFINE_HANDLE(afxTerminal);
AFX_DEFINE_HANDLE(afxFont);
AFX_DEFINE_HANDLE(afxTheme);

typedef enum auxEventId
{
    auxEventId_RUN = 1,
    auxEventId_QUIT,

    auxEventId_KEY,

    // events são mudanças de estado, não continuidade de estado. Use funções para trabalhar com continuidade de estado.
    auxEventId_LMB,
    auxEventId_RMB,
    auxEventId_MMB,
    auxEventId_XMB1,
    auxEventId_XMB2,

    auxEventId_WHEEL, // mouse wheel
    auxEventId_AXIS, // mouse motion

    auxEventId_PLACEMENT,
    auxEventId_MOVE, // Window/widget's position changed.
    auxEventId_SIZE, // Window/widget's size changed.
    auxEventId_FOCUS, // widget focus changed
    auxEventId_FOCUS_LOST, // widget focus changed
    auxEventId_SCROLL, // ex.: list scroll
    auxEventId_CHANGED, // ex.: setting applied
    auxEventId_CHECKED, // ex.: combobox (un)checked
    auxEventId_STYLE, // Window/widget's style has been changed.
    auxEventId_SHOW, // Window/widget was shown on screen.
    auxEventId_PAINT,

    // cursor
    auxEventId_CURS_ENTER, // Cursor enters window/widget's boundaries.
    auxEventId_CURS_MOTION, // Cursor move.
    auxEventId_CURS_HOVER, // pointer enter/leave
    auxEventId_CURS_LEAVE, // Cursor leaves window/widget's boundaries.
    auxEventId_HOVER_IN, // The cursor enters a hover window/widget.
    auxEventId_HOVER_ON, // The cursor moves inside a hover window/widget.
    auxEventId_HOVER_OUT, // The cursor leaves a hover window/widget.
    auxEventId_CLICK_ON, // Cursor press
    auxEventId_CLICK_OFF, // Cursor release
    auxEventId_CLICK_DOUBLE, // Double-click

    // drag and drop
    auxEventId_DRAG_IN, // The cursor enters a window/widget during a drag and drop operation.
    auxEventId_DRAG_ON, // A drag and drop operation is in progress.
    auxEventId_DRAG_OUT, // The cursor leaves a window/widget during a drag and drop operation.
    auxEventId_DROP, // A drag and drop operation is completed.

    auxEventId_CLOSE, // Window/widget was closed.
    auxEventId_ACTIVATE,
} auxEventId;

AFX_DEFINE_STRUCT(auxEvent)
{
    afxEvent        ev;
    auxEventId      id;
    afxUnit          seat;
    afxWindow       wnd;
    afxWidget       wid;
    afxSize         udd;
    afxSize         udd2;
};

AFX_DEFINE_STRUCT(auxInputEvent)
{
    auxEvent        ev;
    
};

#endif//AUX_UX_DEFS_H
