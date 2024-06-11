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

#ifndef AUX_UX_DEFS_H
#define AUX_UX_DEFS_H

#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"

#ifndef __e2ux__
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
#endif//__e2ux__

AFX_DEFINE_HANDLE(afxWidget);
AFX_DEFINE_HANDLE(afxScript);

AFX_DEFINE_HANDLE(afxHandle);

typedef enum afxUxEventId
{
    afxUxEventId_RUN = 1,
    afxUxEventId_QUIT,

    afxUxEventId_KEY,

    // events são mudanças de estado, não continuidade de estado. Use funções para trabalhar com continuidade de estado.
    afxUxEventId_LMB,
    afxUxEventId_RMB,
    afxUxEventId_MMB,
    afxUxEventId_XMB1,
    afxUxEventId_XMB2,

    afxUxEventId_WHEEL, // mouse wheel
    afxUxEventId_AXIS, // mouse motion

    afxUxEventId_HOVER, // pointer enter/leave

    afxUxEventId_MOVE, // widget move
    afxUxEventId_SIZE, // widget resized
    afxUxEventId_FOCUS, // widget focus changed
    afxUxEventId_SCROLL, // ex.: list scroll
    afxUxEventId_CHANGED, // ex.: setting applied
    afxUxEventId_CHECKED, // ex.: combobox (un)checked

    afxUxEventId_DND, // drag and drop


} afxUxEventId;

AFX_DEFINE_STRUCT(afxUxEvent)
{
    afxUxEventId        id;
    afxNat32            tid; // poster
    //afxBool             posted;
    afxBool             accepted;
    afxNat              hidPortIdx;
    afxWindow           wnd;
    afxWidget           wid;
    afxSize             udd;
    afxSize             udd2;
};

#endif//AUX_UX_DEFS_H
