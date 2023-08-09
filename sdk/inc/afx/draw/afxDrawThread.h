/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_DRAW_THREAD_H
#define AFX_DRAW_THREAD_H

#include "afx/draw/afxDrawContext.h"
#include "afx/core/async/afxThread.h"

AFX_DEFINE_STRUCT(afxDrawThreadConfig)
{
    afxThreadConfig  base;
};

#if (defined(_AFX_DRAW_THREAD_C) && !(defined(_AFX_THREAD_C)))
#   error "afxThread not exposed.";
#endif

struct _afxDthrD
#ifdef _AFX_DRAW_THREAD_C    
{
    _AFX_DBG_FCC
    afxDrawThread           dthrObj;
    afxThread               thr; // AFX_FCC_DTHR
    afxDrawDriver           ddrv;
    afxDrawContext          dctx;
    afxNat                  portIdx;
    afxDrawQueue            dque;
    afxNat                  queueIdx;

    afxContext               mem;
}
#endif
;

AFX afxError                AfxAcquireDrawThreads(afxNat cnt, afxDrawThread dthr[], afxDrawThreadConfig const config[]);
AFX void                    AfxReleaseDrawThreads(afxNat cnt, afxDrawThread dthr[]);

AFX afxDrawDriver           AfxGetDrawThreadActiveDriver(afxDrawThread dthr);
AFX afxDrawContext          AfxGetDrawThreadActiveContext(afxDrawThread dthr);
AFX afxDrawQueue            AfxGetDrawThreadActiveQueue(afxDrawThread dthr);

#endif//AFX_DRAW_THREAD_H