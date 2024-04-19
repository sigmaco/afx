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

#ifndef AFX_UX_DEFS_H
#define AFX_UX_DEFS_H

#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"

#ifndef __e2ux__
#   ifdef _DEBUG
#       define AUX _AFXIMPORT extern 
#       define AUXINL _AFXIMPORT extern inline
#   else
#       define AUX _AFXIMPORT extern 
#       define AUXINL _AFXIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _AUX _AFXEXPORT
#       define AUX _AFXEXPORT extern 
#       define _AUXINL _AFXEXPORT inline
#       define AUXINL _AFXEXPORT extern inline
#   else
#       define _AUX _AFXEXPORT
#       define AUX _AFXEXPORT extern 
#       define _AUXINL _AFXEXPORT inline
#       define AUXINL _AFXEXPORT extern inline
#   endif
#endif//__e2ux__

AFX_DEFINE_HANDLE(afxWidget);
AFX_DEFINE_HANDLE(afxOverlay);

AFX_DEFINE_HANDLE(afxHandle);

#endif//AFX_UX_DEFS_H
