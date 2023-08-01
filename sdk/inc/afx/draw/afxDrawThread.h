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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_DRAW_THREAD_H
#define AFX_DRAW_THREAD_H

#include "afx/draw/afxDrawContext.h"
#include "afx/core/async/afxThread.h"

AFX_DEFINE_STRUCT(afxDrawThreadSpecification)
{
    afxThreadSpecification  base;
};

#if (defined(_AFX_DRAW_THREAD_C) && !(defined(_AFX_THREAD_C)))
#   error "afxThread not exposed.";
#endif

AFX_OBJECT(afxDrawThread)
{
    AFX_OBJECT(afxThread)   thr; // AFX_FCC_DTHR
#ifdef _AFX_DRAW_THREAD_C    
    afxDrawDriver           ddrv;
    afxDrawContext          dctx;
    afxNat                  portIdx;
    afxDrawQueue            dque;
    afxNat                  queueIdx;

    afxMemory               mem;
#endif
};

AFX afxDrawDriver           AfxGetDrawThreadActiveDriver(afxDrawThread dthr);
AFX afxDrawContext          AfxGetDrawThreadActiveContext(afxDrawThread dthr);
AFX afxDrawQueue            AfxGetDrawThreadActiveQueue(afxDrawThread dthr);

#endif//AFX_DRAW_THREAD_H