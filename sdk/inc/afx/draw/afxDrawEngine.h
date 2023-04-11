/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_DRAW_QUEUE_H
#define AFX_DRAW_QUEUE_H

#include "afxDrawScript.h"

// No QWADRO, uma fila é dividida em duas partes, afxDrawInput na vanguarda e afxDrawEngine na retaguarda.

AFX_DEFINE_HANDLE(afxDrawEngine);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxDrawEngine)
{
    afxObject           obj; // AFX_FCC_DENG
};

#endif

AFX void* AfxDrawEngineGetContext(afxDrawEngine deng);
AFX void* AfxDrawEngineGetDriver(afxDrawEngine deng);
AFX void* AfxDrawEngineGetDrawSystem(afxDrawEngine deng);

AFX afxError    AfxDrawEngineWaitForIdle(afxDrawEngine deng);

#endif//AFX_DRAW_QUEUE_H