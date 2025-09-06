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

#ifndef AFX_THREAD_POOL_H
#define AFX_THREAD_POOL_H

#include "qwadro/exec/afxThread.h"

#define AFX_MAX_THREAD_POOL_PARALLELISM (32)

AFX_DEFINE_STRUCT(afxThreadedTask)
{
    void(*f)(void*);
    void *arg;
};

AFX_DEFINE_STRUCT(afxThreadPoolInfo)
{
    afxUnit         thrCnt;
    afxUnit         workQueCap;
    afxThreadUsage  usage;
    afxThreadFlags  flags;
    //afxThreadProc procCb;
    afxUnit         tid;
    void*           udd[4];
};

AFX afxInt      AfxPushThreadedTask(afxThreadPool thrp, int flags, void(*f)(void*), void* arg);

////////////////////////////////////////////////////////////////////////////////

AFX afxError    AfxBuildThreadPool(afxHere const hint, afxThreadPoolInfo const* info, afxThreadPool* pool);

#endif//AFX_THREAD_POOL_H
