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

#include "qwadro/core/afxThread.h"
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

AVX afxError            AfxAcquireDrawThreads(afxDrawThreadConfig const* cfg, afxHint const hint, afxNat cnt, afxDrawThread dthr[]);

AVX afxDrawDevice       AfxGetDrawThreadDevice(afxDrawThread dthr);
AVX afxThread           AfxGetDrawThreadBase(afxDrawThread dthr);
AVX void*               AfxGetDrawThreadUdd(afxDrawThread dthr);

AVX afxBool             AfxGetDrawThreadActiveContext(afxDrawThread dthr, afxDrawContext* dctx);
AVX afxBool             AfxGetDrawThreadActiveQueue(afxDrawThread dthr, afxDrawQueue* dque);

#endif//AFX_DRAW_THREAD_H
