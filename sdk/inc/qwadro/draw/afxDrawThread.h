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

// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_THREAD_H
#define AFX_DRAW_THREAD_H

#include "qwadro/async/afxThread.h"
#include "qwadro/draw/afxDrawDefs.h"

AFX_DEFINE_STRUCT(afxDrawThreadConfig)
{
    afxThreadConfig base;
    afxDrawDevice   ddev;
    void*           udd;
};

//#if (defined(_AFX_DRAW_THREAD_C) && !(defined(_AFX_THREAD_C)))
//#   error "afxThread not exposed.";
//#endif

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_THREAD_C
AFX_OBJECT(afxDrawThread)
{
    AFX_OBJECT(afxThread)   thr;
    afxDrawDevice           ddev;
    afxDrawContext          dctx;
    afxNat                  portIdx;
    afxDrawQueue            dque;
    afxNat                  queueIdx;

    void*                   udd;
};
#endif
#endif

AFX afxError            AfxAcquireDrawThreads(afxNat cnt, afxDrawThreadConfig const config[], afxDrawThread dthr[]);

AFX afxDrawDevice       AfxGetDrawThreadDevice(afxDrawThread dthr);
AFX afxThread           AfxGetDrawThreadBase(afxDrawThread dthr);
AFX void*               AfxGetDrawThreadUdd(afxDrawThread dthr);

AFX afxBool             AfxGetDrawThreadActiveContext(afxDrawThread dthr, afxDrawContext* dctx);
AFX afxBool             AfxGetDrawThreadActiveQueue(afxDrawThread dthr, afxDrawQueue* dque);

#endif//AFX_DRAW_THREAD_H