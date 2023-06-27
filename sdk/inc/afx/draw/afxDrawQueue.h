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

#include "afx/draw/afxDrawScript.h"

// No QWADRO, uma fila é dividida em duas partes, afxDrawInput na vanguarda e afxDrawQueue na retaguarda.

AFX_DEFINE_STRUCT(afxDrawQueueSpecification)
{
    afxNat      idx;
    afxBool     autonomous;
};

AFX_DEFINE_HANDLE(afxDrawQueue);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxDrawQueue)
{
    afxObject   obj; // AFX_FCC_DQUE
};

#endif

AFX void*       AfxDrawQueueGetContext(afxDrawQueue dque);
AFX void*       AfxDrawQueueGetDriver(afxDrawQueue dque);
AFX void*       AfxDrawQueueGetDrawSystem(afxDrawQueue dque);

AFX afxError    AfxDrawQueueWaitForIdle(afxDrawQueue dque);

#endif//AFX_DRAW_QUEUE_H