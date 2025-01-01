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
 // Advanced User Experience on Qwadro                                       //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_UX_DEFS_H
#define AUX_UX_DEFS_H

#include "qwadro/inc/exec/afxSystem.h"
#include "qwadro/inc/draw/afxDrawSystem.h"
#include "qwadro/inc/sound/afxSoundSystem.h"

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
AFX_DEFINE_HANDLE(afxSession);
AFX_DEFINE_HANDLE(afxTerminal);

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

    auxEventId_HOVER, // pointer enter/leave

    auxEventId_MOVE, // widget move
    auxEventId_SIZE, // widget resized
    auxEventId_FOCUS, // widget focus changed
    auxEventId_SCROLL, // ex.: list scroll
    auxEventId_CHANGED, // ex.: setting applied
    auxEventId_CHECKED, // ex.: combobox (un)checked

    auxEventId_DND, // drag and drop
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
